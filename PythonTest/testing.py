import numpy as np
import matplotlib.pyplot as plt
from tensorflow import keras
import tkinter as tk
from tkinter import Canvas, Button, Label, Frame
from PIL import Image, ImageDraw
import PIL

# Tải bộ dữ liệu MNIST
def load_and_prepare_mnist():
    (x_train, y_train), (x_test, y_test) = keras.datasets.mnist.load_data()
    x_train = x_train.astype('float32') / 255.0
    x_test = x_test.astype('float32') / 255.0
    x_train = np.expand_dims(x_train, -1)
    x_test = np.expand_dims(x_test, -1)
    y_train = keras.utils.to_categorical(y_train, 10)
    y_test = keras.utils.to_categorical(y_test, 10)
    return (x_train, y_train), (x_test, y_test)

# Xây dựng mô hình CNN
def build_model():
    model = keras.Sequential([
        keras.layers.Conv2D(32, kernel_size=(3, 3), activation='relu', input_shape=(28, 28, 1)),
        keras.layers.MaxPooling2D(pool_size=(2, 2)),
        keras.layers.Conv2D(64, kernel_size=(3, 3), activation='relu'),
        keras.layers.MaxPooling2D(pool_size=(2, 2)),
        keras.layers.Flatten(),
        keras.layers.Dropout(0.5),
        keras.layers.Dense(128, activation='relu'),
        keras.layers.Dense(10, activation='softmax')
    ])
    model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
    return model

# Huấn luyện mô hình
def train_model(model, x_train, y_train, x_test, y_test):
    try:
        model.load_weights('mnist_model.weights.h5')
        print("Đã tải mô hình được huấn luyện trước đó")
    except:
        print("Huấn luyện mô hình mới...")
        model.fit(x_train, y_train, batch_size=128, epochs=5, validation_data=(x_test, y_test))
        model.save_weights('mnist_model.weights.h5')
    return model

# Ứng dụng vẽ
class DrawingApp:
    def __init__(self, model):
        self.model = model
        self.root = tk.Tk()
        self.root.title("Nhận diện chữ số viết tay")
        
        # Frame chính
        main_frame = Frame(self.root)
        main_frame.pack(padx=10, pady=10)
        
        # Canvas để vẽ
        self.canvas_frame = Frame(main_frame)
        self.canvas_frame.grid(row=0, column=0, padx=10, pady=10)
        
        self.canvas = Canvas(self.canvas_frame, width=280, height=280, bg='black')
        self.canvas.pack()
        
        # Frame điều khiển
        self.control_frame = Frame(main_frame)
        self.control_frame.grid(row=0, column=1, padx=10, pady=10)
        
        self.clear_button = Button(self.control_frame, text="Xóa", command=self.clear_canvas)
        self.clear_button.pack(pady=5)
        
        self.predict_button = Button(self.control_frame, text="Dự đoán", command=self.predict)
        self.predict_button.pack(pady=5)
        
        # Nhãn kết quả
        self.result_label = Label(self.control_frame, text="Kết quả: ", font=('Arial', 18))
        self.result_label.pack(pady=20)
        
        # Frame xác suất
        self.probs_frame = Frame(main_frame)
        self.probs_frame.grid(row=1, column=0, columnspan=2, padx=10, pady=10)
        
        self.prob_labels = []
        for i in range(10):
            lbl = Label(self.probs_frame, text=f"Số {i}: 0.00%", width=10)
            lbl.grid(row=0, column=i, padx=5)
            self.prob_labels.append(lbl)
        
        # Khởi tạo hình ảnh để vẽ
        self.image = PIL.Image.new("L", (280, 280), 0)
        self.draw = ImageDraw.Draw(self.image)
        
        # Bind các sự kiện chuột
        self.canvas.bind("<B1-Motion>", self.paint)
        self.canvas.bind("<Button-1>", self.start_paint)  # Bắt đầu vẽ
        self.canvas.bind("<ButtonRelease-1>", self.reset_paint)  # Reset khi nhả chuột
        
        self.old_x = None
        self.old_y = None
        
    def start_paint(self, event):
        self.old_x = event.x
        self.old_y = event.y

    def paint(self, event):
        x, y = event.x, event.y
        if self.old_x and self.old_y:
            self.canvas.create_line(self.old_x, self.old_y, x, y, width=18, fill='white', capstyle=tk.ROUND, smooth=tk.TRUE)
            self.draw.line([self.old_x, self.old_y, x, y], fill=255, width=18)
        self.old_x = x
        self.old_y = y
    
    def reset_paint(self, event):
        self.old_x = None
        self.old_y = None
    
    def clear_canvas(self):
        self.canvas.delete("all")
        self.canvas.configure(bg='black')  # Đặt lại nền đen
        self.image = PIL.Image.new("L", (280, 280), 0)  # Tạo lại hình ảnh mới
        self.draw = ImageDraw.Draw(self.image)
        self.old_x = None
        self.old_y = None
        self.result_label.config(text="Kết quả: ")
        for i in range(10):
            self.prob_labels[i].config(text=f"Số {i}: 0.00%")
    
    def predict(self):
        img = self.image.resize((28, 28), PIL.Image.LANCZOS)
        img_array = np.array(img)
        img_array = img_array.astype('float32') / 255.0
        img_array = np.expand_dims(img_array, axis=0)
        img_array = np.expand_dims(img_array, axis=-1)
        
        predictions = self.model.predict(img_array)
        digit = np.argmax(predictions[0])
        confidence = predictions[0][digit] * 100
        
        self.result_label.config(text=f"Kết quả: {digit} ({confidence:.2f}%)")
        for i in range(10):
            prob = predictions[0][i] * 100
            self.prob_labels[i].config(text=f"Số {i}: {prob:.2f}%")
    
    def run(self):
        self.root.mainloop()




def main():
    print("Tải bộ dữ liệu MNIST...")
    (x_train, y_train), (x_test, y_test) = load_and_prepare_mnist()
    
    print("Xây dựng mô hình...")
    model = build_model()
    
    print("Huấn luyện mô hình...")
    model = train_model(model, x_train, y_train, x_test, y_test)

    score = model.evaluate(x_test, y_test, verbose=0)
    print(f"Độ chính xác trên tập kiểm tra: {score[1] * 100:.2f}%")
    
    print("Khởi động ứng dụng vẽ...")
    app = DrawingApp(model)
    app.run()

if __name__ == "__main__":
    main()