// const quizData = [
//   {
//     topic: "General Knowledge",
//     questions: [
//       {
//         question: "What is the capital of France?",
//         choices: ["Berlin", "Madrid", "Paris", "Rome"],
//         correct: "C",
//       },
//       {
//         question: "What is the largest planet in the solar system?",
//         choices: ["Earth", "Jupiter", "Saturn", "Mars"],
//         correct: "B",
//       },
//       {
//         question: "Who wrote 'To Kill a Mockingbird'?",
//         choices: [
//           "Harper Lee",
//           "J.K. Rowling",
//           "Ernest Hemingway",
//           "Mark Twain",
//         ],
//         correct: "A",
//       },
//     ],
//   },
//   {
//     topic: "Math",
//     questions: [
//       {
//         question: "What is 2 + 2?",
//         choices: ["3", "4", "5", "6"],
//         correct: "B",
//       },
//       {
//         question: "What is 2 * 3?",
//         choices: ["3", "4", "5", "6"],
//         correct: "D",
//       },
//       {
//         question: "What is 2 - 2?",
//         choices: ["0", "1", "2", "3"],
//         correct: "A",
//       },
//     ],
//   },
//   {
//     topic: "Science",
//     questions: [
//       {
//         question: "What is the boiling point of water?",
//         choices: ["50°C", "75°C", "100°C", "150°C"],
//         correct: "C",
//       },
//       {
//         question: "What is the freezing point of water?",
//         choices: ["0°C", "25°C", "50°C", "100°C"],
//         correct: "A",
//       },
//     ],
//   },
//   {
//     topic: "History",
//     questions: [
//       {
//         question: "Who was the first president of the United States?",
//         choices: [
//           "George Washington",
//           "Thomas Jefferson",
//           "Abraham Lincoln",
//           "John Adams",
//         ],
//         correct: "A",
//       },
//       {
//         question: "Who was the first emperor of Rome?",
//         choices: ["Julius Caesar", "Augustus", "Nero", "Caligula"],
//         correct: "B",
//       },
//     ],
//   },
//   {
//     topic: "Geography",
//     questions: [
//       {
//         question: "What is the largest country in the world?",
//         choices: ["Canada", "China", "Russia", "United States"],
//         correct: "C",
//       },
//       {
//         question: "What is the longest river in the world?",
//         choices: ["Amazon", "Congo", "Nile", "Mississippi"],
//         correct: "A",
//       },
//     ],
//   },
//   {
//     topic: "Literature",
//     questions: [
//       {
//         question: "Who wrote 'Pride and Prejudice'?",
//         choices: [
//           "Jane Austen",
//           "Charlotte Bronte",
//           "Emily Dickinson",
//           "Virginia Woolf",
//         ],
//         correct: "A",
//       },
//     ],
//   },
// ];

// const topicContainer = document.querySelector(".Topiccontainer");
// topicContainer.innerHTML = "";
// quizData.forEach((topicData, index) => {
//   topicContainer.innerHTML += `
//       <a href="../Fast Mode/game.html" class="btn">${topicData.topic}</a>
//     `;
// });
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
  button.href = `../Fast Mode/game.html?topic=${encodeURIComponent(
    topicData.topic
  )}`;

  topicContainer.appendChild(button);
});
