import torch
import torch.nn as nn
import torch.optim as optim
import torchvision.transforms as transforms
from torch.utils.data import DataLoader
from torchvision.datasets import ImageFolder
from tqdm import tqdm

# 检查CUDA是否可用，若可用则使用CUDA，否则使用CPU
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# 定义数据转换
data_transforms = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.RandomHorizontalFlip(),
    transforms.ToTensor(),
    transforms.Normalize([0.5, 0.5, 0.5], [0.5, 0.5, 0.5])
])

# 加载数据集
train_dataset = ImageFolder('train_data/train', transform=data_transforms)
test_dataset = ImageFolder('train_data/test', transform=data_transforms)

# 定义数据加载器
train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=32, shuffle=False)

# 定义模型
model = torch.hub.load('pytorch/vision:v0.9.0', 'mobilenet_v3_small', weights=None)
state_dict = torch.load('weights/mobilenet_v3_small-047dcff4.pth', map_location=device)
model.load_state_dict(state_dict)
model.classifier = nn.Linear(576, len(train_dataset.classes))

# 将模型移动到正确的设备上
model.to(device)

# 定义损失函数和优化器
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(), lr=0.0001)

# 获取标签名称
label_names = train_dataset.classes
print('标签名称:', label_names)

# 获取标签数量
num_labels = len(train_dataset.classes)
print('标签数量:', num_labels)

# 训练模型
for epoch in range(100):
    # 训练
    model.train()
    for inputs, labels in tqdm(train_loader):
        optimizer.zero_grad()

        # 将数据移动到正确的设备上
        inputs, labels = inputs.to(device), labels.to(device)

        outputs = model(inputs)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()

    # 测试
    model.eval()
    with torch.no_grad():
        correct = 0
        total = 0
        for inputs, labels in tqdm(test_loader):
            # 将数据移动到正确的设备上
            inputs, labels = inputs.to(device), labels.to(device)

            outputs = model(inputs)
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()

        accuracy = 100 * correct / total
        print('Epoch {}, Test Accuracy: {:.2f}%'.format(epoch, accuracy))

        # 保存模型
    torch.save(model.state_dict(), 'model/weights.pth')