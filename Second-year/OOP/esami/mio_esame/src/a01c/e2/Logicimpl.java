package a01c.e2;

import java.util.HashMap;
import java.util.Map;

public class Logicimpl implements Logic {

	private Map<Pair<Integer, Integer>, String> map = new HashMap<Pair<Integer, Integer>, String>();
	private int size;
	
	public Logicimpl(int size) {
		this.size = size;
	}
	
	public void clicked(int x, int y) {
		if (map.isEmpty()) {
			map.put(new Pair<Integer, Integer>(x, y), "*");
		}
	}
	
	public boolean wasSigned(int x, int y) {
		return map.containsKey(new Pair<Integer, Integer>(x, y));
	}
	
}
