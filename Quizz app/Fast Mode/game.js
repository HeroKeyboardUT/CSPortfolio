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

const questions = topicData.questions;
let currentQuestionIndex = 0;
let score = 0;
let timeRemaining = 30;
let timer;

const questionEl = document.querySelector(".question");
const choicesEl = document.querySelectorAll(".choice");
const timerEl = document.querySelector(".timeshow");
const progressShowEl = document.querySelector(".progressshow");
const scoreShowEl = document.querySelector(".scoreshow");
const taskBarEl = document.querySelector(".task-bar");
const popupEl = document.querySelector(".popup");
const popupMessageEl = document.querySelector(".popup-message");

function loadQuestion() {
  if (currentQuestionIndex >= questions.length) {
    showPopup();
    return;
  }

  const currentQuestion = questions[currentQuestionIndex];
  questionEl.textContent = currentQuestion.question;

  choicesEl.forEach((choice, index) => {
    const answerText = choice.querySelector(".atext");
    if (answerText) {
      answerText.textContent = currentQuestion.choices[index];
    } else {
      choice.textContent = currentQuestion.choices[index];
    }

    choice.classList.remove("correct", "wrong");
    choice.style.pointerEvents = "auto";
  });

  progressShowEl.textContent = `${currentQuestionIndex + 1}/${
    questions.length
  }`;
  taskBarEl.style.width = `${
    ((currentQuestionIndex + 1) / questions.length) * 100
  }%`;
}
function resetTimer() {
  clearInterval(timer);
  timeRemaining = 100;
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

function submitAnswer(selectedAnswerEl) {
  const correctAnswer = questions[currentQuestionIndex].correct;
  const selectedAnswer =
    selectedAnswerEl.querySelector(".atext")?.textContent ||
    selectedAnswerEl.textContent;

  const selectedIndex = Array.from(choicesEl).indexOf(selectedAnswerEl);

  if (selectedIndex === correctAnswer.charCodeAt(0) - 65) {
    selectedAnswerEl.classList.add("correct");
    score++;
    scoreShowEl.textContent = score;
  } else {
    selectedAnswerEl.classList.add("wrong");
  }

  choicesEl.forEach((choice) => (choice.style.pointerEvents = "none"));

  setTimeout(() => moveToNextQuestion(), 1000);
}
function moveToNextQuestion() {
  currentQuestionIndex++;
  loadQuestion();
}

function showPopup() {
  clearInterval(timer);
  popupMessageEl.textContent = `Your score is ${score}/${questions.length}.`;
  popupEl.classList.remove("hidden");
}
function goHome() {
  window.location.href = "../Home/index.html";
}
function restartQuiz() {
  popupEl.classList.add("hidden");
  currentQuestionIndex = 0;
  score = 0;
  resetTimer();
  scoreShowEl.textContent = score;
  loadQuestion();
}

choicesEl.forEach((choice) => {
  choice.addEventListener("click", function () {
    submitAnswer(this);
  });
});

resetTimer();
loadQuestion();
