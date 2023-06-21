import torch
import os
import json
from PIL import Image


class Predict:
    def __init__(self, device, class_json_path, model, my_transforms):

        # select device
        self.device = device
        # create model
        self.model = model
        self.model.eval()

        # load class info
        json_file = open(class_json_path, 'rb')
        self.class_indict = json.load(json_file)
        self.my_transforms = my_transforms

    def transform_image(self, img_path):
        if not os.path.exists(img_path):
            print("file: '{}' dose not exist.".format(img_path))
            return None
        image = Image.open(img_path)
        if image.mode != "RGB":
            raise ValueError("input file does not RGB image...")
        return self.my_transforms(image).unsqueeze(0).to(self.device)

    def get_prediction(self, img_path):
        try:
            tensor = self.transform_image(img_path)
            with torch.no_grad():
                # predict class
                output = torch.squeeze(self.model(tensor)).cpu()
                predict = torch.softmax(output, dim=0)
                predict_cla = torch.argmax(predict).numpy()

            return_info = [self.class_indict[str(predict_cla)],
                           predict[predict_cla].numpy()]
        except Exception as e:
            print(e)
            return_info = None
        return return_info
