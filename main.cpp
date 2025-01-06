#include<bits/stdc++.h>

std::array<uint8_t,10> vals = {
    0b10000,
    0b10010,
    0b11000,
    0b10100,
    0b01100,
    0b11100,
    0b11010,
    0b11001,
    0b10111,
    0b01110,
};

const char *names = "ABCDEFGHIJKLMNOPQRSS";

constexpr uint8_t rev(uint8_t x) {
    return
        ((x & 1) << 4) |
        ((x & 2) << 2) |
        ((x & 4) << 0) |
        ((x & 8) >> 2) |
        ((x & 16) >> 4);
}

uint8_t s2v[21];
uint64_t s2tv[21];

std::unordered_set<uint64_t> visited;

std::unordered_set<uint64_t> solutions;

constexpr uint8_t board_get_row(uint64_t b, int row) {
    row += 5;
    row *= 5;
    return (b & (0b11111ull << row)) >> row;
}
constexpr uint8_t board_get_col(uint64_t b, int col) {
    col *= 5;
    return (b & (0b11111ull << col)) >> col;
}
constexpr uint64_t board_set_row(uint64_t b, int row, uint8_t val) {
    row += 5;
    row *= 5;
    uint64_t mask = ~(0b11111ull << row);
    uint64_t newval = ((uint64_t)val & 0b11111ull) << row;
    return (b & mask) | newval;
}

constexpr uint64_t board_set_col(uint64_t b, int col, uint8_t val) {
    col *= 5;
    uint64_t mask = ~(0b11111ull << col);
    uint64_t newval = ((uint64_t)val & 0b11111ull) << col;
    return (b & mask) | newval;
}

constexpr bool row_interferes_with(uint64_t b, uint8_t row, uint8_t stick, uint8_t col) {
    uint8_t r = board_get_row(b, row);
    if(!r)
        return false;
    uint8_t v = s2v[r];
    bool vset = v & (1 << col);
    bool sset = stick & (1 << row);
    return !(vset ^ sset);
}
constexpr bool col_interferes_with(uint64_t b, uint8_t col, uint8_t stick, uint8_t row) {
    uint8_t c = board_get_col(b, col);
    if(!c)
        return false;
    uint8_t v = s2v[c];
    bool vset = v & (1 << row);
    bool sset = stick & (1 << col);
    return !(vset ^ sset);
}


constexpr uint16_t remove(uint16_t avail, uint8_t stickid) {
    uint16_t psa = (stickid + 1) / 2;
    uint16_t mask = ~(1 << psa);
    return avail & mask;
}


void print_solution(uint64_t board) {
    for(int i = 0; i < 5; i++) {
        auto stick = board_get_col(board, i);
        std::cout << (int)stick << (i < 4 ? "," : "");
    }
    std::cout << " | ";
    for(int i = 0; i < 5; i++) {
        auto stick = board_get_row(board, i);
        std::cout << names[stick-1];
    }
    std::cout << "\n";
}

void solve(uint64_t board, uint16_t avail) {
    if(visited.count(board)) {
        return;
    }
    visited.insert(board);

    if(avail == 0) {
        solutions.insert(board);
        return;
    }

    for(uint8_t stickidx = 1; stickidx <= 19; stickidx++) {
        uint16_t psa = (stickidx + 1) / 2;
        uint16_t stickmask = 1u << psa;
        if(!(avail & stickmask)) {
            continue;
        }

        uint8_t v = s2v[stickidx];
        for(int x = 0; x < 5; x++) {
            if(board_get_col(board, x) != 0) {
                continue;
            }
            bool interferes = false;
            for(int y = 0; !interferes && y < 5; y++) {
                interferes |= row_interferes_with(board, y, v, x);
            }
            if(interferes) {
                continue;
            }
            uint64_t newboard = board_set_col(board, x, stickidx);
            uint16_t newavail = remove(avail, stickidx);
            solve(newboard, newavail);
        }

        for(int y = 0; y < 5; y++) {
            if(board_get_row(board, y) != 0) {
                continue;
            }
            bool interferes = false;
            for(int x = 0; !interferes && x < 5; x++) {
                interferes |= col_interferes_with(board, x, v, y);
            }
            if(interferes) {
                continue;
            }
            uint64_t newboard = board_set_row(board, y, stickidx);
            uint16_t newavail = remove(avail, stickidx);
            solve(newboard, newavail);
        }
    }
}

void usage() {
    std::cout << "Usage: ./solver <ROWS> <COLS>\n";
    std::exit(1);
}

void getBoard(int argc, char* argv[], uint8_t rows[5], uint8_t cols[5]) {
    if (argc != 3) {
        usage();
    }
    std::string rs = argv[1];
    if (rs.length() != 5) {
        usage();
    }

    for (int i = 0; i < 5; i++) {
        if (rs[i] == '_') {
            rows[i] = 0;
        } else if (rs[i] >= 'A' && rs[i] <= 'S') {
            rows[i] = rs[i] - 'A' + 1;
        } else if (rs[i] >= 'a' && rs[i] <= 's') {
            rows[i] = rs[i] - 'a' + 1;
        } else {
            usage();
        }
    }

    std::string cs = argv[2];
    std::stringstream ss(cs);
    std::string token;
    int idx = 0;

    while (std::getline(ss, token, ',') && idx < 5) {
        if (token == "_") {
            cols[idx] = 0;
        } else {
            try {
                cols[idx] = std::stoi(token);
            } catch (const std::exception& e) {
                usage();
            }
        }
        idx++;
    }

    if (idx != 5) {
        usage();
    }

}

int main(int argv, char **argc) {

    s2v[0] = 0;
    for(size_t i = 0; i < vals.size(); i++) {
        auto v = vals[i];
        auto a = names[i*2] - 'A' + 1;
        auto b = names[i*2+1] - 'A' + 1;
        s2v[b] = v;
        s2v[a] = rev(v);
    }

    uint8_t rows[5];
    uint8_t cols[5];
    getBoard(argv, argc, rows, cols);

    uint64_t board = 0;
    uint32_t avail = 0b11111111110;
    for(int i = 0; i < 5; i++) {
        board = board_set_row(board,i, rows[i]);
        avail = remove(avail, rows[i]);

    }
    for(int i = 0; i < 5; i++) {
        board = board_set_col(board,i, cols[i]);
        avail = remove(avail, cols[i]);
    }

    solve(board, avail);

    std::cout << "found " << solutions.size() << " solutions." << std::endl;
    for(auto s : solutions) {
        print_solution(s);
    }
}
