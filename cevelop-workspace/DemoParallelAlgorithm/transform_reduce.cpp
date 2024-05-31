#include <vector>
#include <iostream>
#include <numeric>
#include <execution>
int main()
{
    std::vector<double> xvalues(10007, 2.0), yvalues(10007, 2.0);
    double result = std::transform_reduce(
      std::execution::par,
        xvalues.begin(), xvalues.end(),
        yvalues.begin(), 0.0
    );
    std::cout << result << '\n';
}

