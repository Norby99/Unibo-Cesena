package e1;

public abstract class AbstractBatteryImpl implements Battery{
	
	protected double initialEnergy = 1.0;
	private double currentEnergy;
	private boolean inUse = false;
	
	public AbstractBatteryImpl() {
		this.currentEnergy = initialEnergy;
	}
	
	public void startUse() {
		this.inUse = true;
	}
	
	public void stopUse(final double duration) {
		this.inUse = false;
	}
	
	public boolean isUsed() {
		return this.inUse;
	}
	
	public double getEnergy() {
		return this.currentEnergy;
	}
	
	protected void setCurrentEnergy(double currentEnergy) {
		if (currentEnergy >= 0) { 
			this.currentEnergy = currentEnergy;
		} else {
			this.currentEnergy = 0;
		}
	}

}
