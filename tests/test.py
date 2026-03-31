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


def test_build_wspd_tup_np_return_type():
    pts = make_points()
    result = wspd.build_wspd_tup_np(len(pts), 2, 2.0, pts)
    assert isinstance(result, tuple)
    assert len(result) == 2
    a_pairs, b_pairs = result
    assert isinstance(a_pairs, list)
    assert isinstance(b_pairs, list)
    assert len(a_pairs) == len(b_pairs)
    for arr in a_pairs:
        assert isinstance(arr, np.ndarray)
        assert arr.dtype == np.int32
    for arr in b_pairs:
        assert isinstance(arr, np.ndarray)
        assert arr.dtype == np.int32


def test_build_wspd_tup_np_matches_build_wspd_np():
    pts = make_points()
    result_np = wspd.build_wspd_np(len(pts), 2, 2.0, pts)
    a_pairs, b_pairs = wspd.build_wspd_tup_np(len(pts), 2, 2.0, pts)
    assert len(a_pairs) == len(result_np)
    assert len(b_pairs) == len(result_np)
    for i, (nl, nr) in enumerate(result_np):
        np.testing.assert_array_equal(a_pairs[i], nl)
        np.testing.assert_array_equal(b_pairs[i], nr)


def test_build_wspd_flat_np_return_type():
    pts = make_points()
    result = wspd.build_wspd_flat_np(len(pts), 2, 2.0, pts)
    assert isinstance(result, tuple)
    assert len(result) == 4
    a_flat, b_flat, a_offsets, b_offsets = result
    for arr in result:
        assert isinstance(arr, np.ndarray)
        assert arr.ndim == 1
        assert arr.dtype == np.int32


def test_build_wspd_flat_np_offsets_start_at_zero():
    pts = make_points()
    _, _, a_offsets, b_offsets = wspd.build_wspd_flat_np(len(pts), 2, 2.0, pts)
    assert a_offsets[0] == 0
    assert b_offsets[0] == 0


def test_build_wspd_flat_np_matches_tup_np():
    pts = make_points()
    a_flat, b_flat, a_offsets, b_offsets = wspd.build_wspd_flat_np(len(pts), 2, 2.0, pts)
    a_pairs, b_pairs = wspd.build_wspd_tup_np(len(pts), 2, 2.0, pts)
    assert len(a_offsets) == len(a_pairs)
    assert len(b_offsets) == len(b_pairs)
    for i, (a, b) in enumerate(zip(a_pairs, b_pairs)):
        a_start = a_offsets[i]
        a_end   = a_offsets[i + 1] if i + 1 < len(a_offsets) else len(a_flat)
        b_start = b_offsets[i]
        b_end   = b_offsets[i + 1] if i + 1 < len(b_offsets) else len(b_flat)
        np.testing.assert_array_equal(a_flat[a_start:a_end], a)
        np.testing.assert_array_equal(b_flat[b_start:b_end], b)


test_build_wspd_returns_lists()
test_build_wspd_np_returns_numpy_arrays()
test_both_functions_return_same_indices()
test_build_wspd_tup_np_return_type()
test_build_wspd_tup_np_matches_build_wspd_np()
test_build_wspd_flat_np_return_type()
test_build_wspd_flat_np_offsets_start_at_zero()
test_build_wspd_flat_np_matches_tup_np()

