#include "PmergeMe.hpp"

PMergeMe::PMergeMe(const char** arguments) : arguments(arguments)
{}

PMergeMe::~PMergeMe()
{}

template <typename T>
void PMergeMe::printNumbers(const std::string& message, T& numbers)
{
    typedef typename T::iterator iterator;
    std::cout << message;
    size_t count = 0;
    for (iterator i = numbers.begin(); i != numbers.end(); i++)
    {
        std::cout << *i;
        if (count != numbers.size() - 1)
            std::cout << " ";
        count++;
    }
    std::cout << std::endl;
}

void PMergeMe::parseNumbers()
{
    for (int i = 1; arguments[i]; i++)
    {
        std::string str = arguments[i];
        for (size_t j = 0; j < str.length(); j++)
        {
            if (!isdigit(str[j]) && str[j] != '-' && str[j] != '+')
                throw std::invalid_argument("Error: invalid number => " + str);
        }
        Vnumbers.push_back(atoi(str.c_str()));
        if (std::find(Lnumbers.begin(), Lnumbers.end(), atoi(str.c_str())) != Lnumbers.end())
            throw std::invalid_argument("Error: duplicate number => " + str);
        Lnumbers.push_back(atoi(str.c_str()));
    }
}

template <typename T>
void PMergeMe::swap_pairs(typename T::iterator left, typename T::iterator right, size_t pairs_nb)
{
    for (size_t i = 0; i < pairs_nb; i++)
    {
        std::swap(*left, *right);
        ++left;
        ++right;
    }
}

int	jacobsthal(int index)
{
	return (int)( (pow(-1, index-1) + pow(2, index)) / 3 );
}

template <typename T, typename F>
void	devide_n_conquer(T & main, F & pend, F & new_main, F & oddElement, T & leftOvers, size_t pair_level)
{
	typename T::iterator it = main.begin();
	typename T::iterator end = it + (pair_level - 1);
	typename T::iterator start = it + (pair_level / 2) - 1;
	size_t element_size = pair_level / 2;
	size_t nb_it = 0;


	new_main.push_back(it + element_size - 1);
	new_main.push_back(end);

	it += pair_level;
	nb_it += pair_level;

	while (nb_it < main.size())
	{

		 if (nb_it + (pair_level - 1) < main.size())
		 {
		 	start = it + (pair_level / 2) - 1;
		 	end = it + (pair_level - 1);

		 	pend.push_back(start);
		 	new_main.push_back(end);
		 }


		else
		{

			if (nb_it + element_size - 1 < main.size())
			{
				oddElement.push_back(it + element_size - 1);
				it += element_size;
			}


			if (nb_it < main.size())
				leftOvers.insert(leftOvers.end(), it, main.end());
			return ;
		}
		if (nb_it + pair_level < main.size())
		{
			it += pair_level;
			nb_it += pair_level;
		}
		else
			return ;

	}
}

template <typename T, typename F>
void	insert(T & pend, T & new_main, T & oddElement, F copy)
{
	typename F::iterator copy_it;


	for (typename T::iterator itr = new_main.begin(); itr != new_main.end(); itr++)
		copy.push_back(**itr);


	if (pend.size() == 1)
	{
		copy_it = std::upper_bound(copy.begin(), copy.end() - 1, **(pend.begin()));
		new_main.insert(new_main.begin() + (copy_it - copy.begin()), *(pend.begin()));
		copy.insert(copy_it, **(pend.begin()));
	}


	else if (pend.size() > 1)
	{
		size_t jacobsthal_index = 3;
		size_t count = 0;
		size_t index;
		size_t decrease;
		while (!pend.empty())
		{

			index = jacobsthal(jacobsthal_index) - jacobsthal(jacobsthal_index - 1);
			if (index > pend.size())
				index = pend.size();


			decrease = 0;
			while (index != 0)
			{

				if ((jacobsthal(jacobsthal_index) + count) - decrease <= copy.size())
					copy_it = copy.begin() + (jacobsthal(jacobsthal_index) + count) - decrease;
				else
					copy_it = copy.end();


				copy_it = std::upper_bound(copy.begin(), copy_it, **(pend.begin() + index - 1));


				new_main.insert(new_main.begin() + (copy_it - copy.begin()), *(pend.begin() + index - 1));
				copy.insert(copy_it, **(pend.begin() + index - 1));
				pend.erase(pend.begin() + index - 1, pend.begin() + index);

				count++;
				decrease++;
				index--;
			}
			jacobsthal_index++;
		}
	}


	if (!oddElement.empty())
	{
		copy_it = std::upper_bound(copy.begin(), copy.end(), **(oddElement.begin()));
		new_main.insert(new_main.begin() + (copy_it - copy.begin()), *(oddElement.begin()));
	}
}

template <typename T, typename F>
void	rebuild_main(T & main, F & new_main, T & leftOvers, size_t element_size)
{
	T main_copy;
	typename F::iterator it = new_main.begin();


	while (it != new_main.end())
	{
		main_copy.insert(main_copy.end(), (*it) - (element_size - 1), (*it) + 1);
		it++;
	}


	main_copy.insert(main_copy.end(), leftOvers.begin(), leftOvers.end());

	main = main_copy;
}

template <typename T>
void PMergeMe::merge_insertion_sort(T& numbers, size_t pairs_nb)
{
    if (pairs_nb > numbers.size())
        return;
    typedef typename T::iterator iterator;
    iterator it;
    iterator left;
    iterator right;
    size_t i = 0;
    for (; (i + pairs_nb - 1) < numbers.size(); i += pairs_nb)
    {
        it = numbers.begin();
        std::advance(it, i);
        left = it;
        std::advance(left, pairs_nb / 2 - 1);
        right = it;
        std::advance(right, pairs_nb - 1);
        if (*left > *right)
        {
            std::advance(left, -(pairs_nb / 2 - 1));
            std::advance(right, -(pairs_nb / 2 - 1));
            swap_pairs<T>(left, right, pairs_nb / 2);
        }
    }
    merge_insertion_sort(numbers, pairs_nb * 2);
    std::vector<typename T::iterator> pend;
	std::vector<typename T::iterator> new_main;
	std::vector<typename T::iterator> oddElement;
    size_t element_size = pairs_nb / 2;
	T leftOvers;
	devide_n_conquer(numbers, pend, new_main, oddElement, leftOvers, pairs_nb);
    if (!pend.empty() || !oddElement.empty())
	{
		T copy;
		insert(pend, new_main, oddElement, copy);
		rebuild_main(numbers, new_main, leftOvers, element_size);
	}
}

void PMergeMe::mergeVNumbers()
{
    printNumbers("Before:\t", Vnumbers);
    clock_t start = clock();
    merge_insertion_sort(Vnumbers, 2);
    clock_t end = clock();
    printNumbers("After:\t", Vnumbers);
    double duration = static_cast<double>(end - start) / static_cast<double>(CLOCKS_PER_SEC);
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Time to process a range of " << Vnumbers.size() << " elements with std::vector : " << duration << " s" << std::endl;
}

void PMergeMe::mergeLNumbers()
{
    printNumbers("Before:\t", Lnumbers);
    clock_t start = clock();
    merge_insertion_sort(Lnumbers, 2);
    clock_t end = clock();
    double duration = static_cast<double>(end - start) / static_cast<double>(CLOCKS_PER_SEC);
    std::cout << std::fixed << std::setprecision(6);
    printNumbers("After:\t", Lnumbers);
    std::cout << "Time to process a range of " << Lnumbers.size() << " elements with std::deque : " << duration << " s" << std::endl;
}