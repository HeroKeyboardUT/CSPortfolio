let userQuestions = JSON.parse(localStorage.getItem("userQuizData")) || [];
let mode = localStorage.getItem("quizMode");
let userAnswers = new Array(userQuestions.length).fill(null);
let currentQuestionIndex = 0;
let score = 0;
let timeRemaining = 30;
let timer;
console.log(userQuestions);
// DOM Elements
const questionEl = document.querySelector(".question");
const choicesEl = document.querySelectorAll(".choice");
const timerEl = document.querySelector(".timeshow");
const progressShowEl = document.querySelector(".progressshow");
const scoreShowEl = document.querySelector(".scoreshow");
const taskBarEl = document.querySelector(".task-bar");
const popupEl = document.querySelector(".popup");
const popupMessageEl = document.querySelector(".popup-message");

// Load câu hỏi
function loadQuestion() {
  if (currentQuestionIndex >= userQuestions.length) {
    showPopup();
    return;
  }

  const currentQuestion = userQuestions[currentQuestionIndex];
  questionEl.textContent = currentQuestion.question;

  choicesEl.forEach((choice, index) => {
    const answerText = choice.querySelector(".atext");
    if (answerText) {
      answerText.textContent = currentQuestion.choices[index];
    } else {
      choice.textContent = currentQuestion.choices[index];
    }
    console.log(userAnswers[currentQuestionIndex]);
    choice.classList.remove("correct", "wrong", "selected");
    choice.style.pointerEvents = "auto";
  });

  progressShowEl.textContent = `${currentQuestionIndex + 1}/${
    userQuestions.length
  }`;
  taskBarEl.style.width = `${
    ((currentQuestionIndex + 1) / userQuestions.length) * 100
  }%`;
}

// Reset Timer
function resetTimer() {
  clearInterval(timer);
  timeRemaining = 30;
  timerEl.textContent = timeRemaining;
  timer = setInterval(() => {
    timeRemaining--;
    timerEl.textContent = timeRemaining;
    if (timeRemaining <= 0) {
      clearInterval(timer);
      showPopup();
    }
  }, 1000);
}

// Xử lý chọn đáp án
function submitAnswer(selectedAnswerEl) {
  const correctAnswer = userQuestions[currentQuestionIndex].correct;
  const selectedIndex = Array.from(choicesEl).indexOf(selectedAnswerEl);
  const correctIndex = correctAnswer.charCodeAt(0) - 65;

  // Đánh dấu đúng/sai
  if (selectedIndex === correctIndex) {
    selectedAnswerEl.classList.add("correct");
    score++;
    scoreShowEl.textContent = score;
  } else {
    selectedAnswerEl.classList.add("wrong");
    choicesEl[correctIndex].classList.add("correct");
  }

  // Vô hiệu hóa chọn lại
  choicesEl.forEach((choice) => (choice.style.pointerEvents = "none"));

  setTimeout(() => moveToNextQuestion(), 1000);
}

// Chuyển sang câu tiếp theo
function moveToNextQuestion() {
  currentQuestionIndex++;
  loadQuestion();
}

// Hiển thị kết quả
function showPopup() {
  clearInterval(timer);
  popupMessageEl.textContent = `Your score is ${score}/${userQuestions.length}.`;
  popupEl.classList.remove("hidden");
}

// Quay về trang chủ
function goHome() {
  window.location.href = "../Home/index.html";
}

// Làm lại bài quiz
function restartQuiz() {
  popupEl.classList.add("hidden");
  currentQuestionIndex = 0;
  score = 0;
  resetTimer();
  scoreShowEl.textContent = score;
  loadQuestion();
}

// Gán sự kiện cho các đáp án
choicesEl.forEach((choice) => {
  choice.addEventListener("click", function () {
    submitAnswer(this);
  });
});

// Khởi động quiz
resetTimer();
loadQuestion();
