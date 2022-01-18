package e1;

public class RechargeableBattery extends SimpleBattery {
	
	public static double  DECAYFACTOR = 0.01;

	public RechargeableBattery(double energyDropXmin) {
		super(energyDropXmin);
	}
	
	public void recharge() {
		this.setCurrentEnergy(this.initialEnergy);
		this.initialEnergy -= DECAYFACTOR;
	}

}
