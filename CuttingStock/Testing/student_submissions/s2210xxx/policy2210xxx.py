# from policy import Policy
import random
import numpy as np
from abc import abstractmethod
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

# class Policy2210xxx(Policy):
#     def __init__(self):
#         # Student code here
#         pass

#     def get_action(self, observation, info):
#         # Student code here
#         pass

#     # Student code here
#     # You can add more functions if needed

# ---------------------------------------
# Ví dụ Policy Network dùng Policy Gradient (REINFORCE)
# ---------------------------------------
class CuttingStockPolicyNet(nn.Module):
    def __init__(self, num_products, num_stocks, stock_state_size, hidden_size=128):
        """
        num_products: số loại sản phẩm (giả sử cố định)
        num_stocks: số stock đang có
        stock_state_size: kích thước (số phần tử) của vector trạng thái của stock (sau khi làm phẳng)
        """
        super(CuttingStockPolicyNet, self).__init__()
        self.num_products = num_products
        self.num_stocks = num_stocks
        self.stock_state_size = stock_state_size
        
        # Mỗi sản phẩm: quantity, width, height => 3 features
        self.product_feature_size = 3 * num_products
        # Mỗi stock: 2 features cho kích thước (width, height) + stock_state_size (dạng vector)
        self.stock_feature_size = num_stocks * (2 + stock_state_size)
        
        self.input_size = self.product_feature_size + self.stock_feature_size
        
        self.fc1 = nn.Linear(self.input_size, hidden_size)
        self.fc2 = nn.Linear(hidden_size, hidden_size)
        
        # Head cho product selection (sẽ tạo phân phối softmax trên num_products)
        self.product_head = nn.Linear(hidden_size, num_products)
        # Head cho stock selection (phân phối softmax trên num_stocks)
        self.stock_head = nn.Linear(hidden_size, num_stocks)
        # Head cho position: output trung bình và log_std cho x, y (sử dụng phân phối Gaussian)
        self.pos_mean = nn.Linear(hidden_size, 2)
        self.pos_log_std = nn.Linear(hidden_size, 2)
        
    def forward(self, x):
        h = F.relu(self.fc1(x))
        h = F.relu(self.fc2(h))
        product_logits = self.product_head(h)
        stock_logits = self.stock_head(h)
        pos_mean = self.pos_mean(h)
        pos_log_std = self.pos_log_std(h)
        pos_std = torch.exp(pos_log_std)  # đảm bảo std > 0
        return product_logits, stock_logits, pos_mean, pos_std
    
    def select_action(self, observation):
        """
        Chuyển đổi observation (dạng dict) thành vector đầu vào cho network.
        Giả sử observation có dạng:
            {
              "products": [ {"quantity": int, "size": [w, h]}, ... ],
              "stocks": [ numpy.array, numpy.array, ... ]  # mỗi stock là ma trận trạng thái
            }
        """
        # Xử lý features của products: mỗi sản phẩm gồm (quantity, width, height)
        product_features = []
        for prod in observation["products"]:
            product_features.extend([prod["quantity"], prod["size"][0], prod["size"][1]])
        product_features = np.array(product_features, dtype=np.float32)
        
        # Xử lý features của stocks: với mỗi stock, lấy kích thước (dựa vào shape) và vector trạng thái phẳng
        stock_features = []
        for stock in observation["stocks"]:
            stock_w, stock_h = stock.shape
            stock_features.extend([stock_w, stock_h])
            flattened = stock.flatten()
            # Nếu chiều dài của flattened < stock_state_size, pad thêm 0
            if len(flattened) < self.stock_state_size:
                padded = np.pad(flattened, (0, self.stock_state_size - len(flattened)), mode='constant')
            else:
                padded = flattened[:self.stock_state_size]
            stock_features.extend(padded.tolist())
        stock_features = np.array(stock_features, dtype=np.float32)
        
        # Nối các features lại thành vector đầu vào
        features = np.concatenate([product_features, stock_features])
        features = torch.from_numpy(features).unsqueeze(0)  # (1, input_size)
        
        # Truyền qua network
        product_logits, stock_logits, pos_mean, pos_std = self.forward(features)
        
        # Sample product và stock từ phân phối categorical
        product_dist = torch.distributions.Categorical(logits=product_logits)
        product_action = product_dist.sample()
        stock_dist = torch.distributions.Categorical(logits=stock_logits)
        stock_action = stock_dist.sample()
        
        # Sample vị trí (x, y) từ phân phối Gaussian
        pos_dist = torch.distributions.Normal(pos_mean, pos_std)
        pos_action = pos_dist.sample().squeeze(0)  # tensor 1-d với 2 phần tử
        
        # Tạo action dictionary theo cấu trúc mà môi trường của mày dùng
        action = {
            "product_idx": product_action.item(),
            "stock_idx": stock_action.item(),
            "position": (pos_action[0].item(), pos_action[1].item())
        }
        
        # Tính log probability của action (dùng cho cập nhật gradient sau này)
        log_prob = (product_dist.log_prob(product_action) +
                    stock_dist.log_prob(stock_action) +
                    pos_dist.log_prob(pos_action).sum())
        
        return action, log_prob, features

# ---------------------------------------
# Ví dụ vòng lặp huấn luyện với REINFORCE
# ---------------------------------------
def train_policy_gradient(env, policy_net, optimizer, num_episodes=1000, gamma=0.99):
    """
    env: môi trường tương tác (cần có reset() và step(action))
    policy_net: network chính sách (instance của CuttingStockPolicyNet)
    optimizer: optimizer của PyTorch
    """
    for episode in range(num_episodes):
        obs = env.reset()  # reset môi trường, trả về observation dạng dict
        log_probs = []
        rewards = []
        done = False

        while not done:
            # Lấy action từ policy network
            action, log_prob, _ = policy_net.select_action(obs)
            # Thực hiện action trong môi trường
            next_obs, reward, done, info = env.step(action)
            log_probs.append(log_prob)
            rewards.append(reward)
            obs = next_obs

        # Tính tổng phần thưởng giảm dần (discounted returns)
        returns = []
        R = 0
        for r in reversed(rewards):
            R = r + gamma * R
            returns.insert(0, R)
        returns = torch.tensor(returns)
        # Chuẩn hóa returns để giảm variance
        returns = (returns - returns.mean()) / (returns.std() + 1e-5)

        # Tính loss theo công thức REINFORCE: -sum(log_prob * return)
        loss = 0
        for log_prob, R in zip(log_probs, returns):
            loss -= log_prob * R

        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        if episode % 50 == 0:
            print(f"Episode {episode}, Loss: {loss.item():.4f}, Total Reward: {sum(rewards):.4f}")

# ---------------------------------------
# Giả sử ta có một môi trường giả lập tương thích với giao diện của mày
# (Các hàm reset() và step(action) trả về observation, reward, done, info)
# Ví dụ dưới chỉ là dummy environment để minh họa.
# ---------------------------------------
class DummyCuttingStockEnv:
    def __init__(self):
        # Giả sử mày có 2 sản phẩm và 2 stocks, kích thước stock cố định 10x10
        self.num_products = 2
        self.num_stocks = 2
        self.stock_shape = (10, 10)
        self.stock_state_size = 100  # 10x10
        self.reset()

    def reset(self):
        # Reset sản phẩm: mỗi sản phẩm có quantity ngẫu nhiên và kích thước cố định
        self.products = [
            {"quantity": random.randint(1, 5), "size": [random.randint(2, 4), random.randint(2, 4)]}
            for _ in range(self.num_products)
        ]
        # Reset stocks: mỗi stock là ma trận trạng thái, với -1 cho ô trống, -2 cho ô đã cắt (ban đầu tất cả trống)
        self.stocks = [np.full(self.stock_shape, -1, dtype=np.int32) for _ in range(self.num_stocks)]
        # Giả sử observation là dict chứa products và stocks
        return {"products": self.products, "stocks": self.stocks}

    def step(self, action):
        # action là dict có "product_idx", "stock_idx", "position"
        # Ở đây mình thực hiện một cách đơn giản: nếu hành động hợp lệ, update stock, giảm quantity, và trả về reward dương
        prod_idx = action["product_idx"]
        stock_idx = action["stock_idx"]
        pos_x, pos_y = action["position"]
        prod = self.products[prod_idx]
        prod_w, prod_h = prod["size"]

        stock = self.stocks[stock_idx]
        # Kiểm tra nếu có thể đặt
        if (pos_x + prod_w <= stock.shape[0] and pos_y + prod_h <= stock.shape[1] and
            np.all(stock[pos_x:pos_x+prod_w, pos_y:pos_y+prod_h] == -1) and prod["quantity"] > 0):
            # "Cắt" sản phẩm: đánh dấu các ô đã cắt là -2
            stock[pos_x:pos_x+prod_w, pos_y:pos_y+prod_h] = -2
            prod["quantity"] -= 1
            reward = prod_w * prod_h  # reward theo diện tích (có thể tùy chỉnh)
        else:
            reward = -5  # phạt nếu hành động không hợp lệ

        # Kiểm tra kết thúc episode: giả sử kết thúc khi tất cả sản phẩm đều được cắt
        done = all(prod["quantity"] <= 0 for prod in self.products)
        return ({"products": self.products, "stocks": self.stocks}, reward, done, {})

# ---------------------------------------
# Khởi tạo môi trường và training
# ---------------------------------------
if __name__ == "__main__":
    # Thiết lập tham số cho policy network
    num_products = 2
    num_stocks = 2
    stock_state_size = 100  # với stock có kích thước 10x10

    policy_net = CuttingStockPolicyNet(num_products, num_stocks, stock_state_size, hidden_size=128)
    optimizer = optim.Adam(policy_net.parameters(), lr=1e-3)

    env = DummyCuttingStockEnv()
    train_policy_gradient(env, policy_net, optimizer, num_episodes=500, gamma=0.99)