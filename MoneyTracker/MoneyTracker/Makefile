# ============================================================
#  Makefile — MoneyTracker Pro
# ============================================================

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET   = moneytracker

# Libraries
LIBS = -lmysqlclient -lcurl

# Include paths (sesuaikan jika berbeda di sistem kamu)
INCLUDES = -I/usr/include/mysql \
           -I/usr/include/curl

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

# ── Build ────────────────────────────────────────────────────
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)
	@echo ""
	@echo "  ✅  Build berhasil! Jalankan dengan: ./moneytracker"
	@echo ""

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ── Bersihkan ────────────────────────────────────────────────
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "  🧹  Build files dihapus."

# ── Setup database ───────────────────────────────────────────
db-setup:
	@echo "  🗄️  Membuat database..."
	mysql -u root -p < sql/setup.sql

.PHONY: all clean db-setup
