const express = require('express');

const app = express();

app.get("/", (req, res) => {
  res.send("Hello World!");
});

app.get("/*thepath", (req, res) => {
    res.setHeader("Content-Type", "text/plain");
    res.status(404);
    res.send("Page not found " + req.params.thepath);
});

app.listen(3000, ()=> {
    console.log('Server is running on http://localhost:3000');
});
