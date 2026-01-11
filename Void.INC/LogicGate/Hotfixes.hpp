#pragma once

#include "../Misc/GIncludes.hpp"

struct Hotfix {
    std::string name;
    long double bits;
    float bitMult;
    int written = 0;
    sf::RectangleShape rect;

    Hotfix() : bits(0), bitMult(0), written(0) {}

    Hotfix(std::string n, long double c, float m)
        : name(n), bits(c), bitMult(m), written(0) {
        rect.setSize({ 120.f, 80.f });
        rect.setOutlineColor(sf::Color(243, 238, 225));
        rect.setOutlineThickness(2.f);
    }
};

inline std::vector<Hotfix> hotfixList;
inline const float hfWidth = 120.0f;
inline const float hfHeight = 80.0f;
inline const float hfSpacing = 15.0f;

inline void initHotfixes() {
    // Pricing rebalance for 1.0.0 hotfixes
    hotfixList.push_back({ "init_patch", 100.0L, 0.02f });
    hotfixList.push_back({ "boot_optim", 250.0L, 0.02f });
    hotfixList.push_back({ "cache_clear", 650.0L, 0.02f });
    hotfixList.push_back({ "core_refactor", 1600.0L, 0.02f });
    hotfixList.push_back({ "thread_sync", 4200.0L, 0.02f });
    hotfixList.push_back({ "heap_trim", 11000.0L, 0.02f });
    hotfixList.push_back({ "stack_flush", 28000.0L, 0.02f });
    hotfixList.push_back({ "bit_stable_fix", 70000.0L, 0.02f });
    hotfixList.push_back({ "io_throttle", 175000.0L, 0.02f });
    hotfixList.push_back({ "cycle_reduce", 450000.0L, 0.02f });
    hotfixList.push_back({ "bit_flip_v1", 1000000.0L, 0.02f });
    hotfixList.push_back({ "overflow_ctrl", 2200000.0L, 0.02f });
    hotfixList.push_back({ "registry_tweak", 4800000.0L, 0.02f });
    hotfixList.push_back({ "nullptr_fix", 10000000.0L, 0.02f });
    hotfixList.push_back({ "parity_fix", 20000000.0L, 0.02f });
    hotfixList.push_back({ "mem_compress", 38000000.0L, 0.02f });
    hotfixList.push_back({ "cycle_skip", 70000000.0L, 0.02f });
    hotfixList.push_back({ "runtime_opt", 120000000.0L, 0.02f });
    hotfixList.push_back({ "clock_spd_fix", 200000000.0L, 0.02f });
    hotfixList.push_back({ "bus_overdrive", 320000000.0L, 0.02f });
    hotfixList.push_back({ "ptr_deref_opt", 450000000.0L, 0.02f });
    hotfixList.push_back({ "seg_fault_supp", 600000000.0L, 0.02f });
    hotfixList.push_back({ "bin_compact", 750000000.0L, 0.02f });
    hotfixList.push_back({ "gate_bypass_v1", 850000000.0L, 0.02f });
    hotfixList.push_back({ "fetch_accel", 920000000.0L, 0.02f });
    hotfixList.push_back({ "patch_x86_ext", 950000000.0L, 0.02f });
    hotfixList.push_back({ "mem_map_align", 975000000.0L, 0.02f });
    hotfixList.push_back({ "syscall_bypass", 990000000.0L, 0.02f });
    hotfixList.push_back({ "data_leak_fix", 999000000.0L, 0.02f });
    hotfixList.push_back({ "inst_set_ext", 1000000000.0L, 0.02f });

    // 1.0.2 hotfixes (10b - 100t in cost)
    hotfixList.push_back({ "hypervisor_v2", 10000000000.0L, 0.03f });
    hotfixList.push_back({ "kernel_bypass", 13800000000.0L, 0.03f });
    hotfixList.push_back({ "ring_zero_access", 19000000000.0L, 0.03f });
    hotfixList.push_back({ "vram_injection", 26000000000.0L, 0.03f });
    hotfixList.push_back({ "latency_shred", 36000000000.0L, 0.03f });
    hotfixList.push_back({ "overclock_v3", 50000000000.0L, 0.03f });
    hotfixList.push_back({ "node_clustering", 70000000000.0L, 0.03f });
    hotfixList.push_back({ "logic_gate_v4", 95000000000.0L, 0.03f });
    hotfixList.push_back({ "quantum_state_fix", 130000000000.0L, 0.03f });
    hotfixList.push_back({ "dma_acceleration", 185000000000.0L, 0.03f });
    hotfixList.push_back({ "packet_burst_v5", 255000000000.0L, 0.03f });
    hotfixList.push_back({ "thermal_throttle_off", 350000000000.0L, 0.03f });
    hotfixList.push_back({ "gpu_compute_v6", 485000000000.0L, 0.03f });
    hotfixList.push_back({ "isa_refactoring", 670000000000.0L, 0.03f });
    hotfixList.push_back({ "microcode_unlock", 930000000000.0L, 0.03f });
    hotfixList.push_back({ "cache_coherency_v7", 1300000000000.0L, 0.03f });
    hotfixList.push_back({ "signal_cleaner", 1800000000000.0L, 0.03f });
    hotfixList.push_back({ "branch_pred_v8", 2500000000000.0L, 0.03f });
    hotfixList.push_back({ "multi_die_link", 3500000000000.0L, 0.03f });
    hotfixList.push_back({ "fsb_unlocker", 4800000000000.0L, 0.03f });
    hotfixList.push_back({ "die_shrink_v9", 6700000000000.0L, 0.03f });
    hotfixList.push_back({ "dark_silicon_fix", 9300000000000.0L, 0.03f });
    hotfixList.push_back({ "euv_optimization", 13000000000000.0L, 0.03f });
    hotfixList.push_back({ "clock_domain_v10", 18000000000000.0L, 0.03f });
    hotfixList.push_back({ "tensor_pipeline", 25000000000000.0L, 0.03f });
    hotfixList.push_back({ "atomic_ops_v11", 35000000000000.0L, 0.03f });
    hotfixList.push_back({ "sub_zero_cooling", 48000000000000.0L, 0.03f });
    hotfixList.push_back({ "photonic_bridge", 67000000000000.0L, 0.03f });
    hotfixList.push_back({ "vacuum_state_fix", 93000000000000.0L, 0.03f });
    hotfixList.push_back({ "singularity_boot", 100000000000000.0L, 0.03f });
}

inline void to_json(json& j, const Hotfix& h) {
    j = json{ {"n", h.name}, {"w", h.written} };
}

inline void from_json(const json& j, Hotfix& h) {
    j.at("n").get_to(h.name);
    j.at("w").get_to(h.written);
}