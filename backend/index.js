const express = require("express");
const app = express();
const mysql = require("mysql2");
const cors = require("cors");

app.use(cors());
app.use(express.json());


const db = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "root",  // käytä salasanaa jos sinulla on sellainen
    database: "booksdb"
});

db.connect(err => {
    if (err) {
        console.error(err);
        return;
    }
    console.log("Connected to MySQL");
});

//BOOK CRUD

app.get("/book", (req, res) => {
    db.query("SELECT * FROM book", (err, result) => {
        if (err) throw err;
        res.json(result);
    });
});

app.get("/book/:id", (req, res) => {
    db.query("SELECT * FROM book WHERE id=?", [req.params.id], (err, result) => {
        if (err) throw err;
        res.json(result[0]);
    });
});

app.post("/book", (req, res) => {
    const { title, author } = req.body;
    db.query(
        "INSERT INTO book (title, author) VALUES (?, ?)",
        [title, author],
        (err, result) => {
            if (err) throw err;
            res.json({ id: result.insertId });
        }
    );
});

app.put("/book/:id", (req, res) => {
    const { title, author } = req.body;
    db.query(
        "UPDATE book SET title=?, author=? WHERE id=?",
        [title, author, req.params.id],
        (err) => {
            if (err) throw err;
            res.json({ updated: true });
        }
    );
});

app.delete("/book/:id", (req, res) => {
    db.query("DELETE FROM book WHERE id=?", [req.params.id], (err) => {
        if (err) throw err;
        res.json({ deleted: true });
    });
});

app.listen(3000, () => {
    console.log("Server running on http://localhost:3000");
});

//CAR CRUD

app.get("/car", (req, res) => {
    db.query("SELECT * FROM car", (err, result) => {
        if (err) throw err;
        res.json(result);
    });
});


app.get("/car/:id", (req, res) => {
    db.query("SELECT * FROM car WHERE id=?", [req.params.id], (err, result) => {
        if (err) throw err;
        res.json(result[0]);
    });
});

app.post("/car", (req, res) => {
    const { branch, model } = req.body;
    db.query(
        "INSERT INTO car (branch, model) VALUES (?, ?)",
        [branch, model],
        (err, result) => {
            if (err) throw err;
            res.json({ id: result.insertId });
        }
    );
});

app.put("/car/:id", (req, res) => {
    const { branch, model } = req.body;
    db.query(
        "UPDATE car SET branch=?, model=? WHERE id=?",
        [branch, model, req.params.id],
        (err) => {
            if (err) throw err;
            res.json({ updated: true });
        }
    );
});

app.delete("/car/:id", (req, res) => {
    db.query("DELETE FROM car WHERE id=?", [req.params.id], (err) => {
        if (err) throw err;
        res.json({ deleted: true });
    });
});
