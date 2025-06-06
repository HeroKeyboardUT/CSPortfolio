const topicContainer = document.querySelector(".Topiccontainer");
topicContainer.innerHTML = "";

// Dữ liệu câu hỏi
const quizData = [
  { topic: "General Knowledge" },
  { topic: "Math" },
  { topic: "Science" },
  { topic: "History" },
  { topic: "Geography" },
  { topic: "Literature" },
];

quizData.forEach((topicData) => {
  const button = document.createElement("a");
  button.classList.add("btn");
  button.textContent = topicData.topic;
  button.href = `../Test Mode/gametestmode.html?topic=${encodeURIComponent(
    topicData.topic
  )}`;

  topicContainer.appendChild(button);
});
