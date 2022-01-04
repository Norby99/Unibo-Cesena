package oop.lab03.bank;
import oop.lab03.bank.interfaces.BankAccount;

public class StrictBankAccount  implements BankAccount {
	
	private int userID;
	private double balance;
	private int nTransactions;
	static double ATM_TRANSACTION_FEE = 5;
	static double ATM_VAR_FEE = 0.1;

	public StrictBankAccount(int userId, double initialBalance) {
		this.userID = userId;
		this.balance = initialBalance;
	}

    public void deposit(final int usrID, final double amount) {
    	this.nTransactions++;
    	if (checkUser(usrID)) {
    		this.balance += amount;
    		this.chargeManagementFees(usrID);
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
		this.nTransactions++;
		if (checkUser(usrID) && (this.balance - amount) >= 0) {
    		this.balance -= amount;
    		this.chargeManagementFees(usrID);
    	}
    }

    public void depositFromATM(final int usrID, final double amount) {
    	this.nTransactions++;
    	if (checkUser(usrID)) {
    		this.balance += amount;
    		this.chargeManagementFees(usrID);
    	}
    	
    }

    public void withdrawFromATM(final int usrID, final double amount) {
    	this.nTransactions++;
    	if (checkUser(usrID)) {
    		this.balance -= amount;
    		this.chargeManagementFees(usrID);
    	}
    }

    /* Utility method per controllare lo user */
    private boolean checkUser(final int id) {
        return this.userID == id;
    }

	@Override
	public void chargeManagementFees(int usrID) {
		if (checkUser(usrID)) {
			this.balance -= (ATM_TRANSACTION_FEE + ATM_VAR_FEE * this.getnTransactions());
		}
	}

	@Override
	public int getTransactionsCount() {
		return this.getnTransactions();
	}
}
