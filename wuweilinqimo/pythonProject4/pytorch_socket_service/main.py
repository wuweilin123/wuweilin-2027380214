import threading

from torch import nn

from predict import Predict
from socket_server import Socket_server, onProcess
import torch
import torchvision.transforms as transforms

import os


class_json_path = "./class_indices.json"

assert os.path.exists(class_json_path), "class json path does not exist..."

# select device
device = torch.device("cuda")
# create model
model = torch.hub.load('pytorch/vision:v0.9.0', 'mobilenet_v3_small', weights=None)
model.classifier = nn.Linear(576, 4)
state_dict = torch.load('./weights.pth', map_location=device)
model.load_state_dict(state_dict)
model.to(device)
model.eval()

my_transforms = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
    transforms.Normalize([0.5, 0.5, 0.5], [0.5, 0.5, 0.5])
])


def start_server():

    predictor = Predict(device, class_json_path, model, my_transforms)
    onListener = onProcess(predictor)
    server = Socket_server(onListener=onListener)
    server.start()


def start_server_in_thread():
    thread = threading.Thread(target=start_server)
    thread.start()


if __name__ == "__main__":
    start_server_in_thread()