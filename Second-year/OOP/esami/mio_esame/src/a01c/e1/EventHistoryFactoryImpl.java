package a01c.e1;

import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.function.Supplier;

public class EventHistoryFactoryImpl implements EventHistoryFactory {

	@Override
	public <E> EventHistory<E> fromMap(Map<Double, E> map) {
		return new EventHistoryImpl<E>(map);
	}

	@Override
	public <E> EventHistory<E> fromIterators(Iterator<Double> times, Iterator<E> content) {
		Map<Double, E> map = new HashMap<Double, E>();
		while (times.hasNext() && content.hasNext()) {
			map.put(times.next(), content.next());
		}
		return new EventHistoryImpl<E>(map);
	}

	@Override
	public <E> EventHistory<E> fromListAndDelta(List<E> content, double initial, double delta) {
		Map<Double, E> map = new HashMap<Double, E>();
		double val = initial;
		for (E cont : content) {
			map.put(val, cont);
			val += delta;
		}
		return new EventHistoryImpl<E>(map);
	}

	@Override
	public <E> EventHistory<E> fromRandomTimesAndSupplier(Supplier<E> content, int size) {
		
		Map<Double, E> map = new HashMap<Double, E>();
		for (int i = 0; i < size; i++) {
			map.put(Math.random(), content.get());
		}
		return new EventHistoryImpl<E>(map);
	}

	@Override
	public EventHistory<String> fromFile(String file) throws IOException {
		// TODO Auto-generated method stub
		return null;
	}

}
