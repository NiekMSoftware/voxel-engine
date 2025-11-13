#pragma once

#include <memory>

class IMouse;
class IKeyboard;

class Input {
public:
    Input(std::unique_ptr<const IKeyboard> keyboard, std::unique_ptr<const IMouse> mouse);
    ~Input() = default;

    // Delete copy operations
    Input(const Input &) = delete;
    Input &operator=(const Input &) = delete;

    // Allow move operations
    Input(Input &&) = default;
    Input &operator=(Input &&) = default;

    const IKeyboard &GetKeyboard() const;
    const IMouse &GetMouse() const;

private:
    std::unique_ptr<const IKeyboard> pKeyboard;
    std::unique_ptr<const IMouse> pMouse;
};