const stockContainer = document.getElementsByClassName("stock_container");
const productContainer = document.getElementsByClassName("product_container");
const stock_num = document.getElementsByClassName("stock_num");
const product_num = document.getElementsByClassName("product_num");
let placements = [];

// Tạo input cho stocks theo chế độ (same/different)
function generate_stocks() {
  // Xóa kết quả hiển thị cũ
  document.querySelector(".stock_display").innerHTML = "";
  document.getElementById("placements_list").innerHTML = "";
  let num = parseInt(stock_num.item(0).value) || 0;
  let mode = document.querySelector('input[name="stock_mode"]:checked').value;
  stockContainer.item(0).innerHTML = "";
  if (mode === "same") {
    stockContainer.item(0).innerHTML = `<div class="stock">
            <label>Stock (same for all stocks):</label>
            <input type="number" class="stock_length" placeholder="length" required>
            <input type="number" class="stock_width" placeholder="width" required>
          </div>`;
  } else {
    for (let i = 0; i < num; i++) {
      stockContainer.item(0).innerHTML += `<div class="stock">
              <label>Stock ${i + 1}:</label>
              <input type="number" class="stock_length" placeholder="length" required>
              <input type="number" class="stock_width" placeholder="width" required>
            </div>`;
    }
  }
}

// Tạo input cho products
function generate_products() {
  document.querySelector(".stock_display").innerHTML = "";
  document.getElementById("placements_list").innerHTML = "";
  let num = parseInt(product_num.item(0).value) || 0;
  productContainer.item(0).innerHTML = "";
  for (let i = 0; i < num; i++) {
    productContainer.item(0).innerHTML += `<div class="product">
            <label>Product ${i + 1}:</label>
            <input type="number" class="product_length" placeholder="length" required>
            <input type="number" class="product_width" placeholder="width" required>
            <input type="number" class="product_quantity" placeholder="quantity" required>
          </div>`;
  }
}

function random() {
  document.querySelector(".stock_display").innerHTML = "";
  document.getElementById("placements_list").innerHTML = "";
  stock_num.item(0).value = Math.floor(Math.random() * 20) + 2;
  product_num.item(0).value = Math.floor(Math.random() * 20) + 2;
  generate_stocks();
  generate_products();
  let mode = document.querySelector('input[name="stock_mode"]:checked').value;
  if (mode === "same") {
    let stockLengthInput = document
      .getElementsByClassName("stock_length")
      .item(0);
    let stockWidthInput = document
      .getElementsByClassName("stock_width")
      .item(0);
    stockLengthInput.value = Math.floor(Math.random() * 30) + 30;
    stockWidthInput.value = Math.floor(Math.random() * 30) + 30;
  } else {
    for (
      let i = 0;
      i < document.getElementsByClassName("stock_length").length;
      i++
    ) {
      document.getElementsByClassName("stock_length").item(i).value =
        Math.floor(Math.random() * 30) + 30;
      document.getElementsByClassName("stock_width").item(i).value =
        Math.floor(Math.random() * 30) + 30;
    }
  }
  for (
    let i = 0;
    i < document.getElementsByClassName("product_length").length;
    i++
  ) {
    document.getElementsByClassName("product_length").item(i).value =
      Math.floor(Math.random() * 10) + 5;
    document.getElementsByClassName("product_width").item(i).value =
      Math.floor(Math.random() * 10) + 5;
    document.getElementsByClassName("product_quantity").item(i).value =
      Math.floor(Math.random() * 20) + 1;
  }
}

// Giải bài toán đặt sản phẩm vào stocks
function Solve() {
  placements = [];
  const stocks = [];
  const products = [];
  let mode = document.querySelector('input[name="stock_mode"]:checked').value;

  // Lấy dữ liệu stocks
  if (mode === "same") {
    let numStocks = parseInt(stock_num.item(0).value) || 0;
    let sLength = parseInt(
      document.getElementsByClassName("stock_length").item(0).value
    );
    let sWidth = parseInt(
      document.getElementsByClassName("stock_width").item(0).value
    );
    for (let i = 0; i < numStocks; i++) {
      stocks.push({
        length: sLength,
        width: sWidth,
        matrix: Array.from({ length: sLength }, () => Array(sWidth).fill(-1)),
      });
    }
  } else {
    const stockLengths = document.getElementsByClassName("stock_length");
    const stockWidths = document.getElementsByClassName("stock_width");
    for (let i = 0; i < stockLengths.length; i++) {
      let sLength = parseInt(stockLengths.item(i).value);
      let sWidth = parseInt(stockWidths.item(i).value);
      stocks.push({
        length: sLength,
        width: sWidth,
        matrix: Array.from({ length: sLength }, () => Array(sWidth).fill(-1)),
      });
    }
  }

  // Lấy dữ liệu products
  const prodLengths = document.getElementsByClassName("product_length");
  const prodWidths = document.getElementsByClassName("product_width");
  const prodQuantities = document.getElementsByClassName("product_quantity");
  for (let i = 0; i < prodLengths.length; i++) {
    products.push({
      length: parseInt(prodLengths.item(i).value),
      width: parseInt(prodWidths.item(i).value),
      quantity: parseInt(prodQuantities.item(i).value),
    });
  }

  // Hàm kiểm tra đặt được sản phẩm tại vị trí (x,y)
  function canPlace(stock, x, y, prodLength, prodWidth) {
    if (
      x + prodLength > stock.matrix.length ||
      y + prodWidth > stock.matrix[0].length
    )
      return false;
    for (let i = 0; i < prodLength; i++) {
      for (let j = 0; j < prodWidth; j++) {
        if (stock.matrix[x + i][y + j] !== -1) return false;
      }
    }
    return true;
  }

  // Đánh dấu vùng đã sử dụng
  function markStock(stock, x, y, prodLength, prodWidth) {
    for (let i = 0; i < prodLength; i++) {
      for (let j = 0; j < prodWidth; j++) {
        stock.matrix[x + i][y + j] = 0;
      }
    }
  }

  // Sắp xếp products theo diện tích giảm dần
  products.sort((a, b) => b.length * b.width - a.length * a.width);

  // Đặt products vào stocks
  products.forEach((product) => {
    while (product.quantity > 0) {
      let placed = false;
      for (let stock of stocks) {
        let foundPosition = false;
        for (let x = 0; x < stock.matrix.length; x++) {
          for (let y = 0; y < stock.matrix[0].length; y++) {
            if (canPlace(stock, x, y, product.length, product.width)) {
              markStock(stock, x, y, product.length, product.width);
              placements.push({
                stockIndex: stocks.indexOf(stock),
                productSize: [product.length, product.width],
                position: [x, y],
              });
              product.quantity--;
              placed = true;
              foundPosition = true;
              break;
            }
          }
          if (foundPosition) break;
        }
        if (placed) break;
      }
      if (!placed) {
        console.warn(
          `Không thể đặt hết sản phẩm ${product.length}x${product.width}. Còn lại ${product.quantity}`
        );
        break;
      }
    }
  });
  // console.log("Sắp xếp hoàn thành:", placements);
  displayPlacements();
}

function displayResults() {
  const displayContainer = document.querySelector(".stock_display");
  displayContainer.innerHTML = "";
  const scale = 10; // Hệ số scale chuyển đổi đơn vị -> pixel
  let mode = document.querySelector('input[name="stock_mode"]:checked').value;
  let stocksCount =
    mode === "same"
      ? parseInt(stock_num.item(0).value) || 0
      : document.getElementsByClassName("stock_length").length;

  for (let i = 0; i < stocksCount; i++) {
    let sLength, sWidth;
    if (mode === "same") {
      sLength = parseInt(
        document.getElementsByClassName("stock_length").item(0).value
      );
      sWidth = parseInt(
        document.getElementsByClassName("stock_width").item(0).value
      );
    } else {
      sLength = parseInt(
        document.getElementsByClassName("stock_length").item(i).value
      );
      sWidth = parseInt(
        document.getElementsByClassName("stock_width").item(i).value
      );
    }
    let stockWrapper = document.createElement("div");
    stockWrapper.style.display = "flex";
    stockWrapper.style.flexDirection = "column";
    stockWrapper.style.alignItems = "center";

    let label = document.createElement("div");
    label.classList.add("stock-visual-label");
    label.innerText = `Stock ${i + 1} (${sLength}x${sWidth})`;
    stockWrapper.appendChild(label);

    let stockDiv = document.createElement("div");
    stockDiv.classList.add("stock-visual");
    stockDiv.style.width = sWidth * scale + "px";
    stockDiv.style.height = sLength * scale + "px";
    stockDiv.style.position = "relative";
    stockWrapper.appendChild(stockDiv);

    displayContainer.appendChild(stockWrapper);

    placements.forEach((placement) => {
      if (placement.stockIndex === i) {
        let [pLength, pWidth] = placement.productSize;
        let [x, y] = placement.position;
        let prodDiv = document.createElement("div");
        prodDiv.classList.add("product-visual");
        prodDiv.style.width = pWidth * scale + "px";
        prodDiv.style.height = pLength * scale + "px";
        prodDiv.style.top = x * scale + "px";
        prodDiv.style.left = y * scale + "px";
        // Sử dụng hàm getColorByProductSize thay cho getRandomColor
        prodDiv.style.backgroundColor = getColorByProductSize(pLength, pWidth);
        prodDiv.innerText = `${pLength}x${pWidth}`;
        stockDiv.appendChild(prodDiv);
      }
    });
  }
}

function getColorByProductSize(pLength, pWidth) {
  let area = pLength * pWidth;
  const maxArea = 150; // Giá trị tối đa để map màu (có thể điều chỉnh)
  // Map diện tích sang hue: nếu area = 0 thì hue = 120 (xanh lá), nếu area = maxArea hoặc hơn thì hue = 0 (đỏ)
  let baseHue = 120 - (Math.min(area, maxArea) / maxArea) * 120;

  // Tính tỉ lệ (aspect ratio): nếu là hình vuông thì ratio = 1
  let ratio = pLength / pWidth;
  // Tạo một offset nhỏ dựa trên sự chênh lệch của ratio so với 1.
  // Nếu ratio > 1 (dài hơn rộng) thì offset dương, nếu ratio < 1 (rộng hơn dài) thì offset âm.
  let hueOffset = (ratio - 1) * 20; // Hệ số 20 có thể điều chỉnh để tăng giảm độ lệch màu

  // Tổng hợp hue và đảm bảo nằm trong khoảng [0, 120]
  let hue = baseHue + hueOffset;
  hue = Math.max(0, Math.min(120, hue));

  return `hsl(${hue}, 70%, 50%)`;
}

// Hiển thị danh sách placements dạng text
function displayPlacements() {
  const placementsList = document.getElementById("placements_list");
  placementsList.innerHTML = "";
  placements.forEach((placement, index) => {
    let li = document.createElement("li");
    li.innerText = `#${index + 1} - Stock: ${
      placement.stockIndex + 1
    }, Product: ${placement.productSize[0]}x${
      placement.productSize[1]
    }, Position: (${placement.position[0]}, ${placement.position[1]})`;
    placementsList.appendChild(li);
  });
}
