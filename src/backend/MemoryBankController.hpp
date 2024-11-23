#pragma once

#include <cstdint>

#include "Rom.hpp"
#include "Ram.hpp"

struct MemoryBankController 
{
	virtual uint8_t read(uint16_t adr) const = 0;
	virtual void write(uint8_t b, uint16_t adr) = 0; 
    virtual uint8_t getRomBank() const { return 0; }
    virtual uint8_t getRamBank() const { return 0; }
    virtual ~MemoryBankController() = default;
};

class Mbc1 : public MemoryBankController
{
	public:
    Mbc1(Rom *rom, std::optional<ERam> *ram);
    Mbc1(const Mbc1 &) = delete;
    Mbc1 &operator=(const Mbc1 &) = delete;
    ~Mbc1() override = default;

	uint8_t read(uint16_t adr) const override;
	void write(uint8_t b, uint16_t adr) override;
    uint8_t getRomBank() const override { return romBank_; }
    uint8_t getRamBank() const override { return ramBank_; }

    private:
    void adjustRomBank();

    Rom *rom_;
    std::optional<ERam> *ram_;
	uint8_t ramBank_ {0};
    uint8_t romBank_ {1};
	bool ramEnable_ {false};
	bool ramModeSelect_ {false};
};
