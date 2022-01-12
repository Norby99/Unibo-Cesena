package e1;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Optional;
import java.util.function.Function;
import java.util.function.Predicate;

public class ScannerFactoryImpl implements ScannerFactory {

	@Override
	public <X, Y> Scanner<X, List<Y>> collect(Predicate<X> filter, Function<X, Y> mapper) {
		Scanner<X, List<Y>> scan = new Scanner<>() {

			@Override
			public List<Y> scan(Iterator<X> input) {
				List<Y> tempList = new LinkedList<>(); 
				while (input.hasNext()) {
					X val = input.next();
					if (filter.test(val)) {
						tempList.add(mapper.apply(val));
					}
				}
				return tempList;
			}
			
		};
		return scan;
	}

	@Override
	public <X> Scanner<X, Optional<X>> findFirst(Predicate<X> filter) {
		Scanner<X, Optional<X>> scan = new Scanner<>() {

			@Override
			public Optional<X> scan(Iterator<X> input) {
				Iterator<X> filteredList = collect(filter, x->x).scan(input).iterator();

				if (filteredList.hasNext()) {
					return Optional.of(filteredList.next());
				} else {
					return Optional.empty();
				}
			}
			
		};
		return scan;
	}

	@Override
	public Scanner<Integer, Optional<Integer>> maximalPrefix() {
		Scanner<Integer, Optional<Integer>> scan = new Scanner<>() {

			@Override
			public Optional<Integer> scan(Iterator<Integer> input) {
				Optional<Integer> val = Optional.empty();
				if (input.hasNext()) {
					
					val = Optional.of(input.next());
					while (input.hasNext()) {
						Optional<Integer> nextVal = Optional.of(input.next());
						if (val.get() < nextVal.get()) {
							val = nextVal;
						} else {
							break;
						}
					}			
				}
				return val;
			}

		};
		return scan;
	}

	@Override
	public Scanner<Integer, List<Integer>> cumulativeSum() {
		Scanner<Integer, List<Integer>> scan = new Scanner<>() {

			@Override
			public List<Integer> scan(Iterator<Integer> input) {
				List<Integer> myList = new LinkedList<>();
				
				myList.add(input.next());
				while (input.hasNext()) {
					myList.add(myList.get(myList.size()-1) + input.next());
				}
				
				return myList;
			}

		};
		return scan;
	}

}
