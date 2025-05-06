import librosa
import mido
import numpy as np

audio_file = "test1.mp3"  

# Tải tệp âm thanh và trích xuất nhịp
print("Đang phân tích nhịp của bài hát...")
y, sr = librosa.load(audio_file)  # y: tín hiệu âm thanh, sr: tần số mẫu
tempo, beat_frames = librosa.beat.beat_track(y=y, sr=sr)  # Phát hiện tempo và khung nhịp
beat_times = librosa.frames_to_time(beat_frames, sr=sr)  # Chuyển khung thành thời gian (giây)

# In thông tin nhịp
print(f"Tempo ước lượng: {tempo:.2f} BPM")
print(f"Số nhịp phát hiện: {len(beat_times)}")
print(f"Thời gian nhịp đầu tiên: {beat_times[:5]} giây")

# # Tạo tệp MIDI
# midi_file = mido.MidiFile()
# track = mido.MidiTrack()
# midi_file.tracks.append(track)

# # Thiết lập tempo trong MIDI (tính từ BPM)
# midi_tempo = mido.bpm2tempo(tempo)  # Chuyển BPM thành micro giây mỗi nhịp
# track.append(mido.MetaMessage('set_tempo', tempo=midi_tempo, time=0))

# # Thiết lập nhạc cụ (program change): 118 cho String Slap hoặc 24 cho Acoustic Guitar
# track.append(mido.Message('program_change', program=118, time=0))  # String Slap

# # Thêm nốt guitar tại mỗi thời điểm nhịp
# ticks_per_beat = midi_file.ticks_per_beat  # Số tick mỗi nhịp (mặc định 480)
# note_duration = int(ticks_per_beat * 0.1)  # Thời lượng nốt ngắn (0.1 nhịp)

# for i, beat_time in enumerate(beat_times):
#     # Chuyển thời gian nhịp (giây) thành ticks
#     tick_time = int(mido.second2tick(beat_time, ticks_per_beat, midi_tempo))
    
#     # Thêm nốt MIDI (note_on và note_off)
#     note = 36  # Nốt C2 (thấp, percussive cho string slap)
#     velocity = 100  # Độ lớn nốt
#     track.append(mido.Message('note_on', note=note, velocity=velocity, time=tick_time if i == 0 else 0))
#     track.append(mido.Message('note_off', note=note, velocity=0, time=note_duration))

# # Lưu tệp MIDI
# output_file = "guitar_beat.mid"
# midi_file.save(output_file)
# print(f"Tệp MIDI đã được tạo: {output_file}")
# print("Bạn có thể mở tệp này bằng DAW như Ableton Live hoặc GarageBand để nghe kết quả.")