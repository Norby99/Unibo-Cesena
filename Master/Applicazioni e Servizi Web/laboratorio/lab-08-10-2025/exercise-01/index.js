const express = require("express");
const { Server } = require("socket.io");
const { createServer } = require("http");
const { join } = require("path");

const app = express();
const server = createServer(app);
const io = new Server(server);

// IMPLEMENTATION HERE

app.use(express.static(join(__dirname, "public")));

// Home
app.get("/", (req, res) => {
  res.sendFile(join(__dirname, "public", "index.html"));
});

// IMPLEMENTATION HERE

io.on("connection", (socket) => {
  console.log("a user connected");

// IMPLEMENTATION HERE
});

server.listen(3000, () => {
  console.log("server running at http://localhost:3000");
});
