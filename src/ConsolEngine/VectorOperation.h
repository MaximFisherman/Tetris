namespace vect
{
		template <class ForwardIterator>
			ForwardIterator min_element ( ForwardIterator first, ForwardIterator last )
			{
				ForwardIterator lowest = first;
				if (first == last) return last;
				while (++first != last)
				if (*first < *lowest) 
					lowest = first;
				return lowest;
			}

		template<class ForwardIterator>
			ForwardIterator max_element(ForwardIterator first, ForwardIterator last)
			{
				if (first == last) {
					return last;
				}
				ForwardIterator largest = first;
				++first;
				for (; first != last; ++first) {
					if (*largest < *first) {
						largest = first;
					}
				}
				return largest;
			}
}