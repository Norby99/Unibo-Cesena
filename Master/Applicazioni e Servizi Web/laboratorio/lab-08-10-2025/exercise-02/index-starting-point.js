// server.js
const express = require("express");
const app = express();
const PORT = process.env.PORT || 3000;

app.use(express.json());

// "Database" in memoria
let movies = require("./movies.json");

// HELPERS -----------------------------------------------------------------


// ROUTES ------------------------------------------------------------------

// GET /movies
// support filters: director, year, genre
// support sort: sort=year | -year | title | -title
app.get("/movies", (req, res) => {
  let result = [...movies];

  // Filters
  const { director, year, genre, sort } = req.query;

  if (director) {
    const directorLower = director.toLowerCase();
    result = result.filter((f) =>
      f.director.toLowerCase().includes(directorLower)
    );
  }

  if (year !== undefined) {
    const y = parseIntSafe(year);
    if (y === null)
      return res
        .status(400)
        .json({ error: "year query parameter must be an integer" });
    result = result.filter((f) => f.year === y);
  }

  if (genre) {
    const genreLower = genre.toLowerCase();
    result = result.filter((f) =>
      f.genres.some((g) => g.toLowerCase() === genreLower)
    );
  }

  // Sorting
  if (sort) {
    const fields = sort
      .split(",")
      .map((s) => s.trim())
      .filter(Boolean);
    // support only first field for simplicity, but allow multiple
    result.sort((a, b) => {
      for (const fieldRaw of fields) {
        let desc = false;
        let field = fieldRaw;
        if (fieldRaw.startsWith("-")) {
          desc = true;
          field = fieldRaw.slice(1);
        }

        let va = a[field];
        let vb = b[field];

        if (va === undefined || vb === undefined) continue; // skip this field

        // string compare vs numeric
        if (typeof va === "string") {
          const cmp = va.localeCompare(vb);
          if (cmp !== 0) return desc ? -cmp : cmp;
        } else if (typeof va === "number") {
          if (va !== vb) return desc ? vb - va : va - vb;
        }
      }
      return 0;
    });
  }

  res.json(result);
});

// GET /movies/:id
app.get("/movies/:id", (req, res) => {
  const id = parseIntSafe(req.params.id);
  if (id === null)
    return res.status(400).json({ error: "id must be an integer" });

  const movie = movies.find((f) => f.id === id);
  if (!movie) return res.status(404).json({ error: "Movie not found" });

  res.json(movie);
});

// POST /movies
app.post("/movies", (req, res) => {
  const newMovie = req.body;

  // Basic shape validation (including id presence)
  const errors = validateMovieObject(newMovie, { requireId: true });
  if (errors.length > 0)
    return res
      .status(400)
      .json({ error: "Invalid movie data", details: errors });

  // Unique id check
  if (movies.some((f) => f.id === newMovie.id)) {
    return res
      .status(400)
      .json({ error: `A movie with id ${newMovie.id} already exists` });
  }

  movies.push(newMovie);
  res.status(201).json(newMovie);
});

// PUT /movies/:id
app.put("/movies/:id", (req, res) => {
  const id = parseIntSafe(req.params.id);
  if (id === null)
    return res.status(400).json({ error: "id must be an integer in path" });

  const index = movies.findIndex((f) => f.id === id);
  if (index === -1) return res.status(404).json({ error: "Movie not found" });

  const incoming = req.body;

  // If incoming contains id, ensure it matches path id (or disallow changing id)
  if (incoming.id !== undefined && incoming.id !== id) {
    return res.status(400).json({ error: "Cannot change movie id" });
  }

  // Validate shape: require fields (except id) or accept partial update?
  // The exercise specified PUT (replace/modify) — we will require full validation except id
  const merged = { ...movies[index], ...incoming, id }; // ensure id remains same
  const errors = validateMovieObject(merged, { requireId: true });
  if (errors.length > 0)
    return res
      .status(400)
      .json({ error: "Invalid movie data", details: errors });

  movies[index] = merged;
  res.json(movies[index]);
});

// DELETE /movies/:id
app.delete("/movies/:id", (req, res) => {
  const id = parseIntSafe(req.params.id);
  if (id === null)
    return res.status(400).json({ error: "id must be an integer" });

  const index = movies.findIndex((f) => f.id === id);
  if (index === -1) return res.status(404).json({ error: "Movie not found" });

  movies.splice(index, 1);
  res.status(204).send(); // No Content
});

// GET /movies/stats
app.get("/movies/stats", (req, res) => {
  const totalMovies = movies.length;

  const byDirector = {};
  const byGenre = {};

  for (const f of movies) {
    // byDirector
    byDirector[f.director] = (byDirector[f.director] || 0) + 1;
    // byGenre
    for (const g of f.genres) {
      byGenre[g] = (byGenre[g] || 0) + 1;
    }
  }

  res.json({
    totalMovies,
    byDirector,
    byGenre,
  });
});

// Simple health
app.get("/", (req, res) => res.send("Movies API is up"));

app.listen(PORT, () => {
  console.log(
    `Movies server listening at http://localhost:${PORT} (year constraint: ${MIN_YEAR}-${CURRENT_YEAR})`
  );
});
