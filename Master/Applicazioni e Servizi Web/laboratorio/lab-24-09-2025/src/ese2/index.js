const http = require("http");
const fs = require("fs");
const ejs = require("ejs");

// Funzione per renderizzare EJS
function renderEJS(res, filePath, data = {}) {
    filePath = __dirname + "/" + filePath;
    fs.readFile(filePath, "utf-8", (err, content) => {
        if (err) {
        res.writeHead(500, { "Content-Type": "text/plain" });
        res.end("Errore nel template: " + err);
        return;
        }
        const html = ejs.render(content, data);
        res.writeHead(200, { "Content-Type": "text/html" });
        res.end(html);
    });
}

const server = http.createServer((req, res) => {
  if (req.url === "/") {
    // Homepage
    renderEJS(res, "./views/index.ejs", { name: "User", date: new Date().toLocaleDateString() });

  } else if (req.url === "/users") {
    // Lista utenti
    const users = [
      { name: "Alice", age: 25 },
      { name: "Bob", age: 30 },
      { name: "Charlie", age: 28 },
    ];

    renderEJS(res, "./views/users.ejs", { users });
  } else if (req.url === "/products") {
    // Catalogo prodotti
    const products = [
        { name: "Laptop", price: 1200, available: true },
        { name: "Mouse", price: 25, available: true },
        { name: "Monitor", price: 180, available: false },
        { name: "Tastiera", price: 70, available: true },
    ];
    renderEJS(res, "./views/products.ejs", { products });
  } else {
    res.writeHead(404, { "Content-Type": "text/plain" });
    res.end("Page not found");
  }
});

server.listen(8080, () => {
  console.log("Server running at http://localhost:8080/");
});
