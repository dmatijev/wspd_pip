import numpy as np
import wspd

# The point class is registered with its docstring as the class name (existing behaviour),
# so we locate it dynamically rather than using wspd.point directly.
_point_names = [x for x in dir(wspd) if 'point' in x.lower()]
if not _point_names:
    raise RuntimeError("wspd module does not expose a 'point' class")
_point = getattr(wspd, _point_names[0])


def make_points(n=5, dim=2):
    return [_point([float(i) * (j + 1) for j in range(dim)]) for i in range(n)]


def test_build_wspd_returns_lists():
    pts = make_points()
    result = wspd.build_wspd(len(pts), 2, 2.0, pts)
    assert isinstance(result, list)
    for l, r in result:
        assert isinstance(l, list)
        assert isinstance(r, list)


def test_build_wspd_np_returns_numpy_arrays():
    pts = make_points()
    result = wspd.build_wspd_np(len(pts), 2, 2.0, pts)
    assert isinstance(result, list)
    for l, r in result:
        assert isinstance(l, np.ndarray)
        assert isinstance(r, np.ndarray)
        assert l.dtype == np.int32
        assert r.dtype == np.int32


def test_both_functions_return_same_indices():
    pts = make_points()
    result_lists = wspd.build_wspd(len(pts), 2, 2.0, pts)
    result_numpy = wspd.build_wspd_np(len(pts), 2, 2.0, pts)
    assert len(result_lists) == len(result_numpy)
    for (ll, lr), (nl, nr) in zip(result_lists, result_numpy):
        assert list(nl) == ll
        assert list(nr) == lr


test_build_wspd_returns_lists()
test_build_wspd_np_returns_numpy_arrays()
test_both_functions_return_same_indices()
