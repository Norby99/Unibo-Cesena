-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Creato il: Gen 12, 2024 alle 10:24
-- Versione del server: 10.4.27-MariaDB
-- Versione PHP: 8.2.0

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `pizzeria`
--

-- --------------------------------------------------------

--
-- Struttura della tabella `allergeni`
--

CREATE TABLE `allergeni` (
  `nome` varchar(45) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dump dei dati per la tabella `allergeni`
--

INSERT INTO `allergeni` (`nome`) VALUES
('latticini'),
('noci'),
('pesce'),
('soia'),
('uova');

-- --------------------------------------------------------

--
-- Struttura della tabella `composizione`
--

CREATE TABLE `composizione` (
  `nomePizza` varchar(45) NOT NULL,
  `nomeIngrediente` varchar(45) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dump dei dati per la tabella `composizione`
--

INSERT INTO `composizione` (`nomePizza`, `nomeIngrediente`) VALUES
('capricciosa', 'carciofi'),
('capricciosa', 'funghi'),
('capricciosa', 'mozzarella'),
('capricciosa', 'prosciutto cotto'),
('capricciosa', 'salsa di pomodoro'),
('capricciosa', 'salsiccia'),
('funghi', 'funghi'),
('funghi', 'mozzarella'),
('funghi', 'salsa di pomodoro'),
('margherita', 'mozzarella'),
('margherita', 'salsa di pomodoro'),
('misto funghi', 'funghi'),
('misto funghi', 'funghi porcini'),
('misto funghi', 'funghi trifolati'),
('misto funghi', 'mozzarella'),
('misto funghi', 'salsa di pomodoro'),
('prosciutto crudo', 'mozzarella'),
('prosciutto crudo', 'prosciutto crudo'),
('prosciutto crudo', 'salsa di pomodoro');

-- --------------------------------------------------------

--
-- Struttura della tabella `comprende`
--

CREATE TABLE `comprende` (
  `idOrdine` int(11) NOT NULL,
  `nomePizza` varchar(45) NOT NULL,
  `quantita` int(11) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dump dei dati per la tabella `comprende`
--

INSERT INTO `comprende` (`idOrdine`, `nomePizza`, `quantita`) VALUES
(14, 'capricciosa', 2),
(14, 'funghi', 1),
(16, 'capricciosa', 2),
(16, 'funghi', 1),
(19, 'margherita', 2),
(20, 'prosciutto crudo', 1);

-- --------------------------------------------------------

--
-- Struttura della tabella `fasce_orarie`
--

CREATE TABLE `fasce_orarie` (
  `oraInizio` time NOT NULL,
  `oraFine` time NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dump dei dati per la tabella `fasce_orarie`
--

INSERT INTO `fasce_orarie` (`oraInizio`, `oraFine`) VALUES
('08:00:00', '08:30:00'),
('08:30:00', '09:00:00'),
('09:00:00', '09:30:00'),
('09:30:00', '10:00:00'),
('10:00:00', '10:30:00'),
('10:30:00', '11:00:00'),
('11:00:00', '11:30:00'),
('11:30:00', '12:00:00'),
('12:00:00', '12:30:00'),
('12:30:00', '13:00:00'),
('13:00:00', '13:30:00'),
('13:30:00', '14:00:00'),
('14:00:00', '14:30:00'),
('14:30:00', '15:00:00'),
('15:00:00', '15:30:00'),
('15:30:00', '16:00:00');

-- --------------------------------------------------------

--
-- Struttura della tabella `ingredienti`
--

CREATE TABLE `ingredienti` (
  `nome` varchar(45) NOT NULL,
  `descrizione` varchar(45) NOT NULL,
  `nomeAllergene` varchar(45) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dump dei dati per la tabella `ingredienti`
--

INSERT INTO `ingredienti` (`nome`, `descrizione`, `nomeAllergene`) VALUES
('carciofi', 'carciofi', NULL),
('funghi', 'funghi freschi', NULL),
('funghi porcini', 'funghi porcini', NULL),
('funghi trifolati', 'funghi trifolati', NULL),
('mozzarella', 'mozzarella  proveniente dall\'alto adige', 'latticini'),
('prosciutto cotto', 'prosciutto cotto', NULL),
('prosciutto crudo', 'prosciutto crudo di Parma', NULL),
('salsa di pomodoro', 'pomodori freschi di sicilia', NULL),
('salsiccia', '', NULL);

-- --------------------------------------------------------

--
-- Struttura della tabella `ordini`
--

CREATE TABLE `ordini` (
  `idOrdine` int(11) NOT NULL,
  `idUtente` int(11) NOT NULL,
  `oraInizio` time NOT NULL,
  `dataOrdine` date NOT NULL,
  `note` varchar(200) DEFAULT NULL,
  `nCivicoDiConsegna` int(11) DEFAULT NULL,
  `viaDiConsegna` varchar(45) DEFAULT NULL,
  `processato` tinyint(1) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dump dei dati per la tabella `ordini`
--

INSERT INTO `ordini` (`idOrdine`, `idUtente`, `oraInizio`, `dataOrdine`, `note`, `nCivicoDiConsegna`, `viaDiConsegna`, `processato`) VALUES
(14, 2, '08:00:00', '2024-01-11', '', NULL, NULL, 0),
(16, 2, '12:30:00', '2024-01-11', '', NULL, NULL, 0),
(19, 2, '08:00:00', '2024-01-19', '', NULL, NULL, 0),
(20, 1, '08:00:00', '2024-01-11', '', NULL, NULL, 1);

-- --------------------------------------------------------

--
-- Struttura della tabella `pizze`
--

CREATE TABLE `pizze` (
  `nome` varchar(45) NOT NULL,
  `prezzo` decimal(4,2) NOT NULL,
  `nomeTipo` varchar(45) NOT NULL,
  `vendute` int(10) UNSIGNED NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dump dei dati per la tabella `pizze`
--

INSERT INTO `pizze` (`nome`, `prezzo`, `nomeTipo`, `vendute`) VALUES
('capricciosa', '6.70', 'pizze classiche', 5),
('funghi', '5.50', 'pizze classiche', 3),
('margherita', '4.50', 'pizze classiche', 4),
('misto funghi', '8.00', 'pizze classiche', 1),
('prosciutto crudo', '6.50', 'pizze classiche', 2);

-- --------------------------------------------------------

--
-- Struttura della tabella `prenotazioni_tavolo`
--

CREATE TABLE `prenotazioni_tavolo` (
  `idPrenotazioneTavolo` int(11) NOT NULL,
  `idTavolo` int(11) NOT NULL,
  `dataPrenotazione` date NOT NULL,
  `oraInizio` time NOT NULL,
  `idUtente` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dump dei dati per la tabella `prenotazioni_tavolo`
--

INSERT INTO `prenotazioni_tavolo` (`idPrenotazioneTavolo`, `idTavolo`, `dataPrenotazione`, `oraInizio`, `idUtente`) VALUES
(1, 1, '2024-01-11', '08:00:00', 2);

-- --------------------------------------------------------

--
-- Struttura della tabella `tavoli`
--

CREATE TABLE `tavoli` (
  `idTavolo` int(11) NOT NULL,
  `posizione` varchar(45) NOT NULL,
  `numeroPersone` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dump dei dati per la tabella `tavoli`
--

INSERT INTO `tavoli` (`idTavolo`, `posizione`, `numeroPersone`) VALUES
(1, 'sala principale', 4),
(2, 'sala principale', 4),
(3, 'sala principale', 4),
(4, 'saletta piccola', 8),
(5, 'saletta piccola', 8);

-- --------------------------------------------------------

--
-- Struttura della tabella `tipi_pizza`
--

CREATE TABLE `tipi_pizza` (
  `nomeTipo` varchar(45) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dump dei dati per la tabella `tipi_pizza`
--

INSERT INTO `tipi_pizza` (`nomeTipo`) VALUES
('impasto napoletano'),
('pizze classiche'),
('pizze speciali');

-- --------------------------------------------------------

--
-- Struttura della tabella `utenti`
--

CREATE TABLE `utenti` (
  `idUtente` int(11) NOT NULL,
  `nome` varchar(45) NOT NULL,
  `cognome` varchar(45) NOT NULL,
  `email` varchar(45) NOT NULL,
  `telefono` varchar(45) DEFAULT NULL,
  `numeroCivico` int(11) DEFAULT NULL,
  `via` varchar(45) DEFAULT NULL,
  `cliente` tinyint(1) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dump dei dati per la tabella `utenti`
--

INSERT INTO `utenti` (`idUtente`, `nome`, `cognome`, `email`, `telefono`, `numeroCivico`, `via`, `cliente`) VALUES
(1, 'Marco', 'Verdi', 'marco.verdi@gmail.com', '3453453455', 34, 'via roma', 1),
(2, 'Giulia', 'Rossi', 'giulia.rossi@gmail.com', '3463463466', 43, 'via merlaschio', 1),
(3, 'giorgio', 'bianchi', 'giorgio.bianchi@gmail.com', NULL, NULL, NULL, 0);

--
-- Indici per le tabelle scaricate
--

--
-- Indici per le tabelle `allergeni`
--
ALTER TABLE `allergeni`
  ADD PRIMARY KEY (`nome`);

--
-- Indici per le tabelle `composizione`
--
ALTER TABLE `composizione`
  ADD PRIMARY KEY (`nomePizza`,`nomeIngrediente`),
  ADD KEY `fk_cmpos_ing` (`nomeIngrediente`);

--
-- Indici per le tabelle `comprende`
--
ALTER TABLE `comprende`
  ADD PRIMARY KEY (`idOrdine`,`nomePizza`),
  ADD KEY `fk_cmpren_piz` (`nomePizza`);

--
-- Indici per le tabelle `fasce_orarie`
--
ALTER TABLE `fasce_orarie`
  ADD PRIMARY KEY (`oraInizio`);

--
-- Indici per le tabelle `ingredienti`
--
ALTER TABLE `ingredienti`
  ADD PRIMARY KEY (`nome`),
  ADD KEY `fk_ing_all` (`nomeAllergene`);

--
-- Indici per le tabelle `ordini`
--
ALTER TABLE `ordini`
  ADD PRIMARY KEY (`idOrdine`),
  ADD UNIQUE KEY `idUtente` (`idUtente`,`oraInizio`,`dataOrdine`),
  ADD KEY `fk_ord_fo` (`oraInizio`);

--
-- Indici per le tabelle `pizze`
--
ALTER TABLE `pizze`
  ADD PRIMARY KEY (`nome`),
  ADD KEY `fk_pizz_all` (`nomeTipo`);

--
-- Indici per le tabelle `prenotazioni_tavolo`
--
ALTER TABLE `prenotazioni_tavolo`
  ADD PRIMARY KEY (`idPrenotazioneTavolo`),
  ADD UNIQUE KEY `uk_idUtente_prenotazione_inizio` (`idUtente`,`dataPrenotazione`,`oraInizio`),
  ADD UNIQUE KEY `idTavolo` (`idTavolo`,`dataPrenotazione`,`oraInizio`),
  ADD KEY `fk_pt_fo` (`oraInizio`);

--
-- Indici per le tabelle `tavoli`
--
ALTER TABLE `tavoli`
  ADD PRIMARY KEY (`idTavolo`);

--
-- Indici per le tabelle `tipi_pizza`
--
ALTER TABLE `tipi_pizza`
  ADD PRIMARY KEY (`nomeTipo`);

--
-- Indici per le tabelle `utenti`
--
ALTER TABLE `utenti`
  ADD PRIMARY KEY (`idUtente`),
  ADD UNIQUE KEY `email` (`email`);

--
-- AUTO_INCREMENT per le tabelle scaricate
--

--
-- AUTO_INCREMENT per la tabella `ordini`
--
ALTER TABLE `ordini`
  MODIFY `idOrdine` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=23;

--
-- AUTO_INCREMENT per la tabella `prenotazioni_tavolo`
--
ALTER TABLE `prenotazioni_tavolo`
  MODIFY `idPrenotazioneTavolo` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT per la tabella `tavoli`
--
ALTER TABLE `tavoli`
  MODIFY `idTavolo` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT per la tabella `utenti`
--
ALTER TABLE `utenti`
  MODIFY `idUtente` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- Limiti per le tabelle scaricate
--

--
-- Limiti per la tabella `composizione`
--
ALTER TABLE `composizione`
  ADD CONSTRAINT `fk_cmpos_ing` FOREIGN KEY (`nomeIngrediente`) REFERENCES `ingredienti` (`nome`),
  ADD CONSTRAINT `fk_cmpos_piz` FOREIGN KEY (`nomePizza`) REFERENCES `pizze` (`nome`);

--
-- Limiti per la tabella `comprende`
--
ALTER TABLE `comprende`
  ADD CONSTRAINT `fk_cmpren_ord` FOREIGN KEY (`idOrdine`) REFERENCES `ordini` (`idOrdine`),
  ADD CONSTRAINT `fk_cmpren_piz` FOREIGN KEY (`nomePizza`) REFERENCES `pizze` (`nome`);

--
-- Limiti per la tabella `ingredienti`
--
ALTER TABLE `ingredienti`
  ADD CONSTRAINT `fk_ing_all` FOREIGN KEY (`nomeAllergene`) REFERENCES `allergeni` (`nome`);

--
-- Limiti per la tabella `ordini`
--
ALTER TABLE `ordini`
  ADD CONSTRAINT `fk_ord_fo` FOREIGN KEY (`oraInizio`) REFERENCES `fasce_orarie` (`oraInizio`),
  ADD CONSTRAINT `fk_ord_ut` FOREIGN KEY (`idUtente`) REFERENCES `utenti` (`idUtente`);

--
-- Limiti per la tabella `pizze`
--
ALTER TABLE `pizze`
  ADD CONSTRAINT `fk_pizz_all` FOREIGN KEY (`nomeTipo`) REFERENCES `tipi_pizza` (`nomeTipo`);

--
-- Limiti per la tabella `prenotazioni_tavolo`
--
ALTER TABLE `prenotazioni_tavolo`
  ADD CONSTRAINT `fk_pt_fo` FOREIGN KEY (`oraInizio`) REFERENCES `fasce_orarie` (`oraInizio`),
  ADD CONSTRAINT `fk_pt_tavolo` FOREIGN KEY (`idTavolo`) REFERENCES `tavoli` (`idTavolo`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  ADD CONSTRAINT `fk_pt_utente` FOREIGN KEY (`idUtente`) REFERENCES `utenti` (`idUtente`) ON DELETE NO ACTION ON UPDATE NO ACTION;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
