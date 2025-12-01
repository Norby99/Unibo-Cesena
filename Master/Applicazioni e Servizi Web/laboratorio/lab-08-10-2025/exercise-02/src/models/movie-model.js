movie-model.js
let movies = require("../../movies.json");


const CURRENT_YEAR = new Date().getFullYear();
const MIN_YEAR = 1888;

function validateMovieObject(obj, { requireId = true } = {}) {
  const errors = [];

  if (requireId) {
    if (obj.id === undefined || obj.id === null) errors.push("id is required");
    else if (!Number.isInteger(obj.id)) errors.push("id must be an integer");
  }

  if (!obj.title || typeof obj.title !== "string")
    errors.push("title is required and must be a string");
  if (!obj.director || typeof obj.director !== "string")
    errors.push("director is required and must be a string");

  if (obj.year !== undefined) {
    if (!Number.isInteger(obj.year)) errors.push("year must be an integer");
    else if (obj.year < MIN_YEAR || obj.year > CURRENT_YEAR) {
      errors.push(`year must be between ${MIN_YEAR} and ${CURRENT_YEAR}`);
    }
  } else {
    errors.push("year is required");
  }

  if (!Array.isArray(obj.genres))
    errors.push("genres is required and must be an array");
  else {
    if (obj.genres.length < 1)
      errors.push("genres must contain at least one element");
    if (!obj.genres.every((g) => typeof g === "string"))
      errors.push("each genre must be a string");
    // optional: unique genres
    const uniqueGenres = Array.from(new Set(obj.genres));
    if (uniqueGenres.length !== obj.genres.length)
      errors.push("genres must be unique");
  }

  return errors;
}

function parseIntSafe(v) {
  const n = parseInt(v, 10);
  return Number.isNaN(n) ? null : n;
}

