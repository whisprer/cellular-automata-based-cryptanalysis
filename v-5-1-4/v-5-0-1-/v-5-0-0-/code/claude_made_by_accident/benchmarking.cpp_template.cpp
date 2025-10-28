namespace caca::util {
    template<typename Func, typename... Args>
    auto benchmark(Func&& func, Args&&... args) {
        using namespace std::chrono;
        
        auto start = high_resolution_clock::now();
        std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
        auto end = high_resolution_clock::now();
        
        return duration_cast<microseconds>(end - start).count();
    }
}