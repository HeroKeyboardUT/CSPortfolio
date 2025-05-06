import pandas as pd
import matplotlib.pyplot as plt

# Đọc tệp Excel
file_path = 'DATA.xlsx'  # Đổi thành đường dẫn của file nếu cần
data = pd.read_excel(file_path)

# Loại bỏ khoảng trắng thừa trong tên cột
data.columns = data.columns.str.strip()

# Tạo một danh sách các cột cần phân tích
columns_to_analyze = [
    "Nhu cầu thành đạt",
    "Chấp nhận những điều chưa rõ ràng",
    "Tầm nhìn (định hướng tương lai)",
    "Sự kiên định",
    "Hiệu quả cá nhân",
    "Tính linh hoạt"
]

# Subset dữ liệu để phân tích
riasec_scores = data[columns_to_analyze]

# Đảm bảo các giá trị là số (chuyển đổi nếu cần)
riasec_scores = riasec_scores.apply(pd.to_numeric, errors='coerce')

# Định nghĩa ngưỡng phân loại
low_threshold = 2.5
high_threshold = 4

# Phân loại điểm theo nhóm
score_distribution = riasec_scores.apply(lambda x: pd.cut(
    x, bins=[0, low_threshold, high_threshold, x.max()],
    labels=["Low (<2.5)", "Medium (2.5-4)", "High (>4)"]
)).apply(pd.Series.value_counts).T



# 3. Average score for each RIASEC type
average_scores = riasec_scores.mean()

# high_need = riasec_scores["Nhu cầu thành đạt"] > high_threshold
# high_acceptance = riasec_scores["Chấp nhận những điều chưa rõ ràng"] > high_threshold
# high_vision = riasec_scores["Tầm nhìn (định hướng tương lai)"] > high_threshold
# high_persistence = riasec_scores["Sự kiên định"] > high_threshold
# high_efficiency = riasec_scores["Hiệu quả cá nhân"] > high_threshold
# high_flexibility = riasec_scores["Tính linh hoạt"] > high_threshold
# high_scores = riasec_scores[high_need & high_acceptance & high_vision & high_persistence & high_efficiency & high_flexibility].shape[0]

# In ra phân phối điểm
print("Phân phối điểm:")
print(score_distribution)
print("\nĐiểm trung bình cho mỗi yếu tố:")
print(average_scores)
# print("\nSố lượng sinh viên có điểm cao:")
# print(high_scores)
# Vẽ đồ thị phân phối điểm
ax = average_scores.plot(kind='barh', stacked=False)
plt.title("Phân phối điểm theo các yếu tố")
plt.xlabel("Điểm trung bình")
plt.ylabel("Yếu tố")
for container in ax.containers:
    ax.bar_label(container, label_type='center')    
plt.show()