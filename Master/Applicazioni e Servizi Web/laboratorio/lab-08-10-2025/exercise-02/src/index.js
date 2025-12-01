// server.js
const express = require("express");
const { CURRENT_YEAR, MIN_YEAR } = require("./models/movie-model");

const app = express();
const PORT = process.env.PORT || 3000;

app.use(express.json());

const movieRoutes = require("./routes/movie-routes");
app.use("/movies", movieRoutes);

app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT} (year constraint: ${MIN_YEAR}-${CURRENT_YEAR})`);
});
