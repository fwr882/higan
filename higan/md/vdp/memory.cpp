auto VDP::VRAM::read(uint16 address) const -> uint16 {
  if(mode == 0) {
    return memory[(uint15)address];
  } else {
    uint15 offset = address >> 1 & 0x7e00 | address & 0x01fe | address >> 9 & 1;
    uint8 data = memory[offset].byte(!address.bit(0));
    return data << 8 | data << 0;
  }
}

auto VDP::VRAM::write(uint16 address, uint16 data) -> void {
  if(mode == 0) {
    memory[(uint15)address] = data;
  } else {
    uint15 offset = address >> 1 & 0x7e00 | address & 0x01fe | address >> 9 & 1;
    memory[offset].byte(!address.bit(0)) = data.byte(0);
  }
  if(address < vdp.sprite.io.nametableAddress) return;
  if(address > vdp.sprite.io.nametableAddress + 319) return;
  vdp.sprite.write(address - vdp.sprite.io.nametableAddress, data);
}

auto VDP::VRAM::readByte(uint17 address) const -> uint8 {
  return read(address >> 1).byte(!address.bit(0));
}

auto VDP::VRAM::writeByte(uint17 address, uint8 data) -> void {
  auto word = read(address >> 1);
  word.byte(!address.bit(0)) = data;
  write(address >> 1, word);
}

auto VDP::VSRAM::read(uint6 address) const -> uint10 {
  if(address >= 40) return 0x0000;
  return memory[address];
}

auto VDP::VSRAM::write(uint6 address, uint10 data) -> void {
  if(address >= 40) return;
  memory[address] = data;
}

auto VDP::CRAM::read(uint6 address) const -> uint9 {
  return memory[address];
}

auto VDP::CRAM::write(uint6 address, uint9 data) -> void {
  memory[address] = data;
}
