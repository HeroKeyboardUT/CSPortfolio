// Lấy dữ liệu từ localStorage
let userQuestions = JSON.parse(localStorage.getItem("userQuizData")) || [];

// Sử dụng userQuestions thay cho quizData
let userAnswers = Array(userQuestions.length).fill(null);
let timerInterval;
let timeLeft = 500;

function generateQuiz() {
  const container = document.getElementsByClassName("quiz-container")[0];
  container.innerHTML = "";

  userQuestions.forEach((item, index) => {
    const questionDiv = document.createElement("div");
    questionDiv.classList.add("question");
    questionDiv.innerHTML = `<h1>${index + 1}. ${item.question}</h1>`;

    item.choices.forEach((choice, choiceIndex) => {
      const choiceDiv = document.createElement("div");
      choiceDiv.classList.add("choice");
      choiceDiv.innerHTML = `
        <p class="answer">${String.fromCharCode(65 + choiceIndex)}</p>
        <p class="atext">${choice}</p>
      `;

      choiceDiv.addEventListener("click", () =>
        handleChoice(index, choiceIndex)
      );
      questionDiv.appendChild(choiceDiv);
    });
    container.appendChild(questionDiv);
  });
}

function handleChoice(questionIndex, choiceIndex) {
  userAnswers[questionIndex] = choiceIndex;
  const questionDiv = document.querySelectorAll(".question")[questionIndex];
  const choices = questionDiv.querySelectorAll(".choice");

  choices.forEach((choice, index) => {
    choice.classList.remove("selected");
    if (index === choiceIndex) {
      choice.classList.add("selected");
    }
  });
}

function calculateScore() {
  let score = 0;
  userAnswers.forEach((answer, index) => {
    const question = userQuestions[index];
    const selectedChoice = document
      .querySelectorAll(".question")
      [index].querySelectorAll(".choice")[answer];

    if (answer !== null) {
      const isCorrect = String.fromCharCode(65 + answer) === question.correct;
      selectedChoice.classList.add(isCorrect ? "correct" : "wrong");
      if (isCorrect) score++;
    }
  });
  return score;
}

function showPopup(message) {
  clearInterval(timerInterval);
  document.querySelector(".popup-message").innerHTML = message;
  document.querySelector(".popup").classList.remove("hidden");
}

function showResults() {
  const score = calculateScore();
  showPopup(`<p>Correct answers: ${score}/${userQuestions.length}</p>`);
}

document.querySelector(".submit-btn").addEventListener("click", () => {
  // Vô hiệu hóa các lựa chọn khi bài đã được nộp
  document.querySelectorAll(".choice").forEach((choice) => {
    choice.style.pointerEvents = "none";
  });
  showResults();
});

function restartQuiz() {
  location.reload();
}

function goHome() {
  window.location.href = "../Home/index.html";
}

// Hàm đếm ngược thời gian, cập nhật giao diện và tự động nộp khi hết giờ
function startTimer() {
  // Cập nhật thời gian ban đầu
  document.querySelector(".timeshow").textContent = timeLeft;
  timerInterval = setInterval(() => {
    timeLeft--;
    document.querySelector(".timeshow").textContent = timeLeft;

    if (timeLeft <= 0) {
      clearInterval(timerInterval);
      // Vô hiệu hóa các lựa chọn khi hết thời gian
      document.querySelectorAll(".choice").forEach((choice) => {
        choice.style.pointerEvents = "none";
      });
      showResults();
    }
  }, 1000);
}

window.onload = () => {
  generateQuiz();
  startTimer();
};
