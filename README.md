# Well-separated pair decomposition (WSPD) 
WSPD is a well-known geometric decomposition used for encoding distances, introduced in 1995 by Paul B. Callahan and S. Rao Kosaraju in a seminal paper. 
WSPD compresses $O(n^2)$ pairwise distances of $n$ given points in $\mathbb{R}^d$ in $O(n)$ space and $O(n\log n)$ construction time for a fixed dimension $d$. We implemented this remarkable decomposition following the nontrivial 
algorithm for computing a partial fair split tree of a point set presented in the original paper. Our implementation is 
done in C++. In addition, we published it as a Python package on PyPi that can be easily accessed on Windows, Linux, or macOS by researchers 
and students worldwide (with Python version >= 3.7). 


To use WSPD as a Python package, simple run 

```
pip install wspd
```

In Python notebook /example/example.ipynb you can find an easy demonstration of how the wspd module can be used. 
