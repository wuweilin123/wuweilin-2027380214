import torch
import torch.nn as nn
import torchvision.transforms as transforms
from PIL import Image
import matplotlib.pyplot as plt
from torchvision.datasets import ImageFolder
plt.rcParams['font.sans-serif']=['SimHei']  # 设置中文字体
plt.rcParams['axes.unicode_minus'] = False  # 解决负号显示问题

# 检查CUDA是否可用，若可用则使用CUDA，否则使用CPU
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# 加载模型
model = torch.hub.load('pytorch/vision:v0.9.0', 'mobilenet_v3_small', weights=None)
model.classifier = nn.Linear(576, 4)
state_dict = torch.load('model/weights.pth', map_location=device)
model.load_state_dict(state_dict)
model.to(device)
model.eval()

# 定义数据转换
data_transforms = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
    transforms.Normalize([0.5, 0.5, 0.5], [0.5, 0.5, 0.5])
])

# 加载图像并进行预测
image_path = 'train_data/test/逆光/0_QG75K_Line_2_20220401112834_ZR019_19_JNP.jpg'
image = Image.open(image_path).convert('RGB')
image_tensor = data_transforms(image).unsqueeze(0).to(device)
with torch.no_grad():
    outputs = model(image_tensor)
    probs = torch.nn.functional.softmax(outputs, dim=1)[0]
    _, predicted = torch.max(outputs.data, 1)

# 获取标签名称
train_dataset = ImageFolder('train_data/train', transform=data_transforms)
label_names = train_dataset.classes

# 将预测结果转换为标签名称
predicted_label = label_names[predicted.item()]
print(predicted_label)
# 显示图像和预测结果
fig, ax = plt.subplots()
ax.imshow(image)
ax.set_title('Predicted Label: {}, Probability: {:.2f}%'.format(predicted_label, probs[predicted.item()] * 100))
plt.show()