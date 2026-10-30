#pragma once



// ---- 64-bit mixer (SplitMix64 finalizer) — strong avalanche, cheap ----
static inline uint64_t mix64(uint64_t v) {
    v += 0x9E3779B97F4A7C15ULL;
    v = (v ^ (v >> 30)) * 0xBF58476D1CE4E5B9ULL;
    v = (v ^ (v >> 27)) * 0x94D049BB133111EBULL;
    return v ^ (v >> 31);
}

// ---- Quantize a coordinate so nearly-equal doubles hash identically ----
static inline int64_t quantize(double c, double tol) {
    return static_cast<int64_t>(std::llround(c / tol));
}

// ---- Hash one vertex ----
static inline uint64_t hashPoint(const Point3& p, double tol) {
    uint64_t h = mix64(static_cast<uint64_t>(quantize(p.x, tol)));
    h = mix64(h ^ static_cast<uint64_t>(quantize(p.y, tol)));
    h = mix64(h ^ static_cast<uint64_t>(quantize(p.z, tol)));
    return h;
}

// ---- Hash a triangle into [0, tableSize) ----
// orderIndependent = true  -> (A,B,C), (B,C,A), (C,B,A)... all give the same index
// orderIndependent = false -> winding/order matters
inline uint32_t hashTriangle(const Point3& a, const Point3& b, const Point3& c,
    uint32_t tableSize = 1'000'000,
    double tol = 1e-6,
    bool orderIndependent = true) {
    uint64_t h[3] = { hashPoint(a, tol), hashPoint(b, tol), hashPoint(c, tol) };
    if (orderIndependent) std::sort(h, h + 3);          // canonical order

    uint64_t key = mix64(h[0]);
    key = mix64(key ^ h[1]);
    key = mix64(key ^ h[2]);

    // Fast, unbiased range reduction: (key * tableSize) >> 64
    return static_cast<uint32_t>((static_cast<std::_Unsigned128>(key) * tableSize) >> 64);
}