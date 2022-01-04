package oop.lab03;

public class StricktBankAccount {
	
	private int userID;
	private double balance;
	private int nTransactions;
	static double ATM_TRANSACTION_FEE = 1;

	public StricktBankAccount(int userId, double initialBalance) {
		this.userID = userId;
		this.balance = initialBalance;
	}

    /*
     * Si aggiungano selettori per: 
     * - ottenere l'id utente del possessore del conto
     * - ottenere il numero di transazioni effettuate
     * - ottenere l'ammontare corrente del conto.
     */

    public void deposit(final int usrID, final double amount) {
        /*
         * Incrementa il numero di transazioni e aggiunge amount al totale del
         * conto Nota: il deposito va a buon fine solo se l'id utente
         * corrisponde
         */
    	this.nTransactions++;
    	if (checkUser(usrID)) {
    		this.balance += amount;
    	}
    }

    public int getUserID() {
		return userID;
	}

	public double getBalance() {
		return balance;
	}

	public int getnTransactions() {
		return nTransactions;
	}

	public void withdraw(final int usrID, final double amount) {
        /*
         * Incrementa il numero di transazioni e rimuove amount al totale del
         * conto. Note: - Il conto puo' andare in rosso (ammontare negativo) -
         * Il prelievo va a buon fine solo se l'id utente corrisponde
         */
		this.nTransactions++;
		if (checkUser(usrID)) {
    		this.balance -= amount;
    	}
    }

    public void depositFromATM(final int usrID, final double amount) {
        /*
         * Incrementa il numero di transazioni e aggiunge amount al totale del
         * conto detraendo le spese (costante ATM_TRANSACTION_FEE) relative
         * all'uso dell'ATM (bancomat) Nota: il deposito va a buon fine solo se
         * l'id utente corrisponde
         */
    	this.nTransactions++;
    	if (checkUser(usrID)) {
    		this.balance += amount - ATM_TRANSACTION_FEE;
    	}
    	
    }

    public void withdrawFromATM(final int usrID, final double amount) {
        /*
         * Incrementa il numero di transazioni e rimuove amount + le spese
         * (costante ATM_TRANSACTION_FEE) relative all'uso dell'ATM (bancomat)
         * al totale del conto. Note: - Il conto puo' andare in rosso (ammontare
         * negativo) - Il prelievo va a buon fine solo se l'id utente
         * corrisponde
         */
    	this.nTransactions++;
    	if (checkUser(usrID)) {
    		this.balance -= amount - ATM_TRANSACTION_FEE;
    	}
    }

    /* Utility method per controllare lo user */
    private boolean checkUser(final int id) {
        return this.userID == id;
    }

	@Override
	public void chargeManagementFees(int usrID) {
		// this class has no charge fees
		
	}

	@Override
	public int getTransactionsCount() {
		return this.getnTransactions();
	}
}
