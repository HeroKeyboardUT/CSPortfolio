import cv2
from deepface import DeepFace
import numpy as np
import matplotlib.pyplot as plt

# Hàm hiển thị cảm xúc lên ảnh
def display_emotion(image_path):
    # Đọc ảnh từ đường dẫn
    img = cv2.imread(image_path)
    
    # Chuyển đổi từ BGR sang RGB
    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    
    # Phát hiện khuôn mặt và phân tích cảm xúc
    try:
        result = DeepFace.analyze(img_path=image_path, actions=['emotion'], enforce_detection=True)
        
        # Nếu có nhiều khuôn mặt, result sẽ là một list
        if isinstance(result, list):
            results = result
        else:
            results = [result]
            
        # Xử lý từng khuôn mặt
        for i, result in enumerate(results):
            # Lấy thông tin khuôn mặt
            face_data = result['region']
            x, y, w, h = face_data['x'], face_data['y'], face_data['w'], face_data['h']
            
            # Vẽ hình chữ nhật xung quanh khuôn mặt
            cv2.rectangle(img_rgb, (x, y), (x+w, y+h), (0, 255, 0), 2)
            
            # Lấy cảm xúc và độ tin cậy
            emotion = result['dominant_emotion']
            emotion_score = result['emotion'][emotion]
            
            # Vẽ nhãn cảm xúc và độ tin cậy
            text = f"{emotion}: {emotion_score:.2f}%"
            cv2.putText(img_rgb, text, (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 0), 2)
            
            # In kết quả phân tích
            print(f"Khuôn mặt {i+1}:")
            print(f"  Cảm xúc: {emotion}")
            print("  Chi tiết các cảm xúc:")
            for emo, score in result['emotion'].items():
                print(f"    - {emo}: {score:.2f}%")
            print()
            
        # Hiển thị ảnh với các khuôn mặt và cảm xúc
        plt.figure(figsize=(10, 8))
        plt.imshow(img_rgb)
        plt.axis('off')
        plt.title('Nhận diện cảm xúc')
        plt.show()
        
    except Exception as e:
        print(f"Lỗi: {str(e)}")
        print("Không tìm thấy khuôn mặt trong ảnh hoặc có lỗi xảy ra.")

# Hàm nhận diện cảm xúc qua webcam
def emotion_recognition_webcam():
    # Khởi tạo webcam
    cap = cv2.VideoCapture(0)
    
    if not cap.isOpened():
        print("Không thể mở webcam")
        return
    
    print("Đang khởi động nhận diện cảm xúc... (Nhấn 'q' để thoát)")
    
    while True:
        # Đọc frame từ webcam
        ret, frame = cap.read()
        
        if not ret:
            print("Không thể đọc từ webcam")
            break
            
        # Tạo một bản sao để hiển thị
        display_frame = frame.copy()
        
        try:
            # Phân tích cảm xúc (có thể mất một chút thời gian)
            result = DeepFace.analyze(frame, actions=['emotion'], enforce_detection=False, detector_backend='opencv')
            
            # Nếu có nhiều khuôn mặt, result sẽ là một list
            if isinstance(result, list):
                results = result
            else:
                results = [result]
                
            # Xử lý từng khuôn mặt
            for result in results:
                # Lấy thông tin khuôn mặt
                face_data = result['region']
                x, y, w, h = face_data['x'], face_data['y'], face_data['w'], face_data['h']
                
                # Vẽ hình chữ nhật xung quanh khuôn mặt
                cv2.rectangle(display_frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
                
                # Lấy cảm xúc và độ tin cậy
                emotion = result['dominant_emotion']
                emotion_score = result['emotion'][emotion]
                
                # Vẽ nhãn cảm xúc và độ tin cậy
                text = f"{emotion}: {emotion_score:.2f}%"
                cv2.putText(display_frame, text, (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 0), 2)
                
        except Exception:
            # Nếu không phát hiện được khuôn mặt, chỉ hiển thị frame gốc
            pass
            
        # Hiển thị frame
        cv2.imshow('Nhận diện cảm xúc qua khuôn mặt', display_frame)
        
        # Nhấn 'q' để thoát
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
            
    # Giải phóng tài nguyên
    cap.release()
    cv2.destroyAllWindows()

def main():
    print("CHƯƠNG TRÌNH NHẬN DIỆN CẢM XÚC QUA KHUÔN MẶT")
    print("1. Nhận diện từ file ảnh")
    print("2. Nhận diện qua webcam")
    
    choice = input("Chọn chức năng (1 hoặc 2): ") 
    
    if choice == '1':
        image_path = input("Nhập đường dẫn đến file ảnh: ")
        display_emotion(image_path)
    elif choice == '2':
        emotion_recognition_webcam()
    else:
        print("Lựa chọn không hợp lệ!")

if __name__ == "__main__":
    main()