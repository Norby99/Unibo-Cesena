package e1;

public class BatteryFactoryImpl implements BatteryFactory {

	@Override
	public Battery createSimpleBattery() {
		return new SimpleBattery(1.0);
	}

	@Override
	public Battery createSimpleBatteryByDrop(double energyPerDurationDrop) {
		return null;
	}

	@Override
	public Battery createSimpleBatteryWithExponentialDrop() {
		return new SimpleBatteryExponentialDrop();
	}

	@Override
	public Battery createSecureBattery() {
		return new SecureBattery(1.0);
	}

	@Override
	public Battery createRechargeableBattery() {
		return new RechargeableBattery(1.0);
	}

	@Override
	public Battery createSecureAndRechargeableBattery() {
		// TODO Auto-generated method stub
		return null;
	}

}
