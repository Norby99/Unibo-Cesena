const express = require("express");
const { Server } = require("socket.io");
const { createServer } = require("http");
const { join } = require("path");

const app = express();
const server = createServer(app);
const io = new Server(server);

let messages = [];
let users = {};

app.use(express.static(join(__dirname, "public")));

// Home
app.get("/", (req, res) => {
  res.sendFile(join(__dirname, "public", "index.html"));
});

io.on("connection", (socket) => {
  console.log("a user connected");

  // Send current history (last N messages)
  socket.emit("history", messages);

  socket.on("set nickname", (nickname) => {
    socket.nickname = nickname || 'Anonimo';
    users[socket.id] = socket.nickname;

    console.log(`User set nickname: ${socket.nickname}`);

    // Broadcast join message and updated user list
    io.emit("chat message", `${socket.nickname} joined the chat`);
    io.emit("users", Object.values(users));
  });

  socket.on("chat message", (msg) => {
    const from = socket.nickname || 'Anonimo';
    const fullMsg = `${from}: ${msg}`;

    messages.push(fullMsg);
    // Limit message history to avoid unbounded memory growth
    const MAX_HISTORY = 500;
    if (messages.length > MAX_HISTORY) messages = messages.slice(-MAX_HISTORY);

    io.emit("chat message", fullMsg);
  });

  socket.on("typing", (isTyping) => {
    // emit who is typing to other clients
    socket.broadcast.emit("typing", socket.nickname || 'Anonimo');
  });

  socket.on("disconnect", () => {
    console.log("user disconnected");
    if (socket.nickname) {
      io.emit("chat message", `${socket.nickname} left the chat`);
      delete users[socket.id];
      io.emit("users", Object.values(users));
    }
  });

});

server.listen(3000, () => {
  console.log("server running at http://localhost:3000");
});
