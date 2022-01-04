package oop.lab03.bank.interfaces;

import oop.lab03.bank.AccountHolder;
import oop.lab03.bank.SimpleBankAccount;
import oop.lab03.bank.StrictBankAccount;

public final class TestBankAccount {

    private TestBankAccount() {
    }

    public static void main(final String[] args) {
        /*
         * 1) Creare l' AccountHolder relativo a Mario Rossi con id 1
         */
    	AccountHolder marioRossi = new AccountHolder("Mario", "Rossi", 1);
        /*
         * 2) Creare l' AccountHolder relativo a Luigi Bianchi con id 2
         */
    	AccountHolder luigiBianchi = new AccountHolder("Luigi", "Bianchi", 2);
        /*
         * 3) Dichiarare due variabili di tipo BankAccount ed inizializzarle,
         * rispettivamente, con oggetti di tipo SimpleBankAccount per il conto di Mario
         * Rossi (ammontare iniziale = 0), e di tipo StrictBankAccount per il conto di
         * Luigi Bianchi (ammontare iniziale = 0)
         */
    	BankAccount marioRossiAccount = new SimpleBankAccount(marioRossi.getUserID(), 0);
    	BankAccount luigiBianchiAccount = new StrictBankAccount(luigiBianchi.getUserID(), 0);
        /*
         * 4) Prima riflessione: perché è stato possibile fare la new di due classi
         * diverse in variabili dello stesso tipo?
         */
        /*
         * 5) Depositare €10000 in entrambi i conti
         */
    	marioRossiAccount.deposit(marioRossi.getUserID(), 10000);
    	luigiBianchiAccount.deposit(luigiBianchi.getUserID(), 10000);
        /*
         * 6) Prelevare €15000 da entrambi i conti
         */
    	marioRossiAccount.withdraw(marioRossi.getUserID(), 15000);
    	luigiBianchiAccount.withdraw(luigiBianchi.getUserID(), 15000);
        /*
         * 7) Stampare in stdout l'ammontare corrente
         */
    	System.out.println("Mario Rossi balance = " + marioRossiAccount.getBalance() + "\n");
    	System.out.println("Luigi Bianchi balance = " + luigiBianchiAccount.getBalance() + "\n");
        /*
         * 8) Qual è il risultato e perché?
         */
        /*
         * 9) Depositare nuovamente €10000 in entrambi i conti
         */
    	marioRossiAccount.deposit(marioRossi.getUserID(), 10000);
    	luigiBianchiAccount.deposit(luigiBianchi.getUserID(), 10000);
        /*
         * 10) Invocare il metodo computeManagementFees su entrambi i conti
         */
    	marioRossiAccount.chargeManagementFees(marioRossi.getUserID());
    	luigiBianchiAccount.chargeManagementFees(luigiBianchi.getUserID());
        /*
         * 11) Stampare a video l'ammontare corrente
         */
    	System.out.println("Mario Rossi balance = " + marioRossiAccount.getBalance() + "\n");
    	System.out.println("Luigi Bianchi balance = " + luigiBianchiAccount.getBalance() + "\n");
        /*
         * 12) Qual è il risultato e perché?
         */

    }
}
