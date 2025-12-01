// movie-routes.js
const express = require("express");
const movieControllers = require("../controllers/movie-controller");

const router = express.Router();

router.get("/", movieControllers.getMovies);

router.get("/movies/stats", movieControllers.getMoviesStats);

router.

module.exports = router;
