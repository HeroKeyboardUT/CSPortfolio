// Mảng chứa các câu hỏi do người dùng tạo
let userCreatedQuestions = [];

// Lấy các phần tử DOM
const questionForm = document.getElementById("question-form");
const finishQuizButton = document.getElementById("finish-quiz");
const modeSelectionDiv = document.getElementById("mode-selection");
const createQuizSection = document.getElementById("create-quiz-section");
const questionsListDiv = document.getElementById("questions-list");

// Hàm cập nhật danh sách câu hỏi hiển thị bên dưới form
function displayQuestions() {
  questionsListDiv.innerHTML = "<h2>Added Questions:</h2>";
  userCreatedQuestions.forEach((q, index) => {
    const questionItem = document.createElement("div");
    questionItem.classList.add("question-item");

    const questionHeading = document.createElement("h3");
    questionHeading.textContent = `Q${index + 1}: ${q.question}`;
    questionItem.appendChild(questionHeading);

    const ul = document.createElement("ul");
    q.choices.forEach((choice, idx) => {
      const li = document.createElement("li");
      const letter = String.fromCharCode(65 + idx);
      if (letter === q.correct) {
        li.innerHTML = `<span class='correct-choice'>${letter}. ${choice} (Correct)</span>`;
      } else {
        li.textContent = `${letter}. ${choice}`;
      }
      ul.appendChild(li);
    });
    questionItem.appendChild(ul);

    questionsListDiv.appendChild(questionItem);
  });
}

// Xử lý khi người dùng submit form để thêm câu hỏi
questionForm.addEventListener("submit", function (e) {
  e.preventDefault();

  const questionText = document.getElementById("question-text").value.trim();
  const choiceA = document.getElementById("choice-A").value.trim();
  const choiceB = document.getElementById("choice-B").value.trim();
  const choiceC = document.getElementById("choice-C").value.trim();
  const choiceD = document.getElementById("choice-D").value.trim();
  const correctAnswer = document.getElementById("correct-answer").value;

  const questionObj = {
    question: questionText,
    choices: [choiceA, choiceB, choiceC, choiceD],
    correct: correctAnswer,
  };

  userCreatedQuestions.push(questionObj);
  questionForm.reset();
  // alert("Question added!");
  // localStorage.setItem("userQuizData", JSON.stringify(data));
  displayQuestions();
});

// Khi nhấn "Finish and Choose Mode"
finishQuizButton.addEventListener("click", function () {
  if (userCreatedQuestions.length === 0) {
    alert("Please add at least one question before finishing.");
    return;
  }
  localStorage.setItem("userQuizData", JSON.stringify(userCreatedQuestions));
  createQuizSection.style.display = "none";
  modeSelectionDiv.style.display = "block";
});

console.log(userCreatedQuestions);
