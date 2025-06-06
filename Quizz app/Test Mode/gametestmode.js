const quizData = [
  {
    topic: "General Knowledge",
    questions: [
      {
        question: "What is the capital of France?",
        choices: ["Berlin", "Madrid", "Paris", "Rome"],
        correct: "C",
      },
      {
        question: "What is the largest planet in the solar system?",
        choices: ["Earth", "Jupiter", "Saturn", "Mars"],
        correct: "B",
      },
      {
        question: "Who wrote 'To Kill a Mockingbird'?",
        choices: [
          "Harper Lee",
          "J.K. Rowling",
          "Ernest Hemingway",
          "Mark Twain",
        ],
        correct: "A",
      },
    ],
  },
  {
    topic: "Math",
    questions: [
      {
        question: "What is 2 + 2?",
        choices: ["3", "4", "5", "6"],
        correct: "B",
      },
      {
        question: "What is 2 * 3?",
        choices: ["3", "4", "5", "6"],
        correct: "D",
      },
      {
        question: "What is 2 - 2?",
        choices: ["0", "1", "2", "3"],
        correct: "A",
      },
    ],
  },
  {
    topic: "Science",
    questions: [
      {
        question: "What is the boiling point of water?",
        choices: ["50°C", "75°C", "100°C", "150°C"],
        correct: "C",
      },
      {
        question: "What is the freezing point of water?",
        choices: ["0°C", "25°C", "50°C", "100°C"],
        correct: "A",
      },
    ],
  },
  {
    topic: "History",
    questions: [
      {
        question: "Who was the first president of the United States?",
        choices: [
          "George Washington",
          "Thomas Jefferson",
          "Abraham Lincoln",
          "John Adams",
        ],
        correct: "A",
      },
      {
        question: "Who was the first emperor of Rome?",
        choices: ["Julius Caesar", "Augustus", "Nero", "Caligula"],
        correct: "B",
      },
    ],
  },
  {
    topic: "Geography",
    questions: [
      {
        question: "What is the largest country in the world?",
        choices: ["Canada", "China", "Russia", "United States"],
        correct: "C",
      },
      {
        question: "What is the longest river in the world?",
        choices: ["Amazon", "Congo", "Nile", "Mississippi"],
        correct: "A",
      },
    ],
  },
  {
    topic: "Literature",
    questions: [
      {
        question: "Who wrote 'Pride and Prejudice'?",
        choices: [
          "Jane Austen",
          "Charlotte Bronte",
          "Emily Dickinson",
          "Virginia Woolf",
        ],
        correct: "A",
      },
    ],
  },
];
// Hàm lấy giá trị tham số từ URL
function getQueryParam(param) {
  const urlParams = new URLSearchParams(window.location.search);
  return urlParams.get(param);
}

const selectedTopic = getQueryParam("topic");
const topicData = quizData.find((q) => q.topic === selectedTopic);

if (!topicData) {
  alert("Invalid topic!");
  window.location.href = "../Home/index.html";
}

// Sử dụng topicData thay cho quizData[0]
let userAnswers = Array(topicData.questions.length).fill(null);
let timerInterval;
let timeLeft = 500;

function generateQuiz() {
  const container = document.getElementsByClassName("quiz-container")[0];
  container.innerHTML = "";

  topicData.questions.forEach((item, index) => {
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
    const question = topicData.questions[index];
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
  showPopup(`<p>Correct answers: ${score}/${topicData.questions.length}</p>`);
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
