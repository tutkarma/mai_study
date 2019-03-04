#include <iostream>
#include <vector>

int main()
{
    std::vector<double> A = {0.0, 2.0, 4.0, -3.0, 3.0};
    std::vector<double> B = {15.0, -15.0, 11.0, 16.0, 8.0};
    std::vector<double> C = {8.0, 4.0, 5.0, -7.0, 0.0};
    std::vector<double> D = {92.0, -84.0, -77.0, 15.0, -11.0};
    std::vector<double> P, Q, X(D.size());

    P.push_back(-C[0]/B[0]);
    Q.push_back(D[0]/B[0]);
    int n = D.size() - 1;

    for (int i = 1; i <= n; ++i) {
        double p_i = (i == n) ? 0 : (-C[i] / (B[i] + A[i] * P[i - 1]));
        double q_i = (D[i] - A[i] * Q[i - 1]) / (B[i] + A[i] * P[i - 1]);
        P.push_back(p_i);
        Q.push_back(q_i);
    }


    X[n] = Q[n];
    for (int i = n - 1; i >= 0; --i) {
        X[i] = P[i] * X[i + 1] + Q[i];
    }

    for (int i = 0; i <= n; ++i) {
        std::cout << "x" << i + 1 << " = " << X[i] << '\n';
    }

    return 0;
}