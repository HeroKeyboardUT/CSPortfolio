import pandas as pd
import matplotlib.pyplot as plt

# Load the cleaned RIASEC data from the Excel file
file_path = 'DATA.xlsx'  # Đổi thành đường dẫn của file nếu cần
data = pd.read_excel(file_path)

# Filter columns related to scores only
columns_to_analyze = [
    "Realistic (Người thực tế)",
    "Investigative (Người thích nghiên cứu)",
    "Artistic (Người có tính nghệ sĩ)",
    "Social (Người có tính xã hội)",
    "Enterprising (Người dám nghĩ dám làm)",
    "Conventional (Người công chức)"
]

# Subset the data for analysis
riasec_scores = data[columns_to_analyze]

# Define thresholds for analysis
low_threshold = 15
high_threshold = 25

# 1. Distribution of scores across all RIASEC types
score_distribution = riasec_scores.apply(lambda x: pd.cut(
    x, bins=[0, low_threshold, high_threshold, x.max()],
    labels=["Low (<15)", "Medium (15-25)", "High (>25)"]
).value_counts()).T

# 2. Count students with Enterprising score >= 25 (potential for entrepreneurship)
entrepreneurial_students = riasec_scores[
    riasec_scores["Enterprising (Người dám nghĩ dám làm)"] >= high_threshold
].shape[0]

# 3. Average score for each RIASEC type
average_scores = riasec_scores.mean()

# Identify students with high scores in Realistic, Enterprising, and Social
high_realistic = riasec_scores["Realistic (Người thực tế)"] > high_threshold
high_enterprising = riasec_scores["Enterprising (Người dám nghĩ dám làm)"] > high_threshold
high_social = riasec_scores["Social (Người có tính xã hội)"] > high_threshold

# Count students meeting each condition
students_high_res = riasec_scores[high_realistic & high_enterprising & high_social].shape[0]

# print("Number of students with high scores in Realistic, Enterprising, and Social:", students_high_res)

# # Print the results
# print("Score Distribution:")
# print(score_distribution)
# print("\nNumber of Entrepreneurial Students (Enterprising >= 25):", entrepreneurial_students)
# print("\nAverage Scores for Each RIASEC Type:")
# print(average_scores)
ax = score_distribution.plot(kind='barh', stacked=False)
plt.title("Phân phối điểm theo các yếu tố")
plt.xlabel("Số lượng")
plt.ylabel("Yếu tố")

# Hiển thị số trong các thành phần của biểu đồ
for container in ax.containers:
    if container!=0:
        ax.bar_label(container, label_type='center')
    else:
        continue

plt.show()