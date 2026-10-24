#pragma once

#include <JuceHeader.h>

// パラメーターセット用のカスタムLookAndFeel
class ParameterLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ParameterLookAndFeel()
    {
        // 背景色を #454545 に設定（暗めのグレー）
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0x45, 0x45, 0x45));
        
        // 非選択時: グレー背景(#454545)に白テキスト
        // 選択時: 白背景(#d9d9d9)にグレーのテキスト
        setColour(juce::TextButton::buttonColourId, juce::Colour(0x45, 0x45, 0x45));
        setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xd9, 0xd9, 0xd9));
        setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        setColour(juce::TextButton::textColourOnId, juce::Colour(0x45, 0x45, 0x45));
        
        // Save/Compileボタンの特別な色を設定
        saveButtonColour = juce::Colour(0xb8, 0x74, 0x74); // #b87474 - 淡い赤
        compileButtonColour = juce::Colour(0x74, 0x95, 0xb8); // #7495b8 - 淡い青
    }
    
    // ボタンタイプの識別
    enum ButtonType {
        NORMAL,
        SAVE,
        COMPILE
    };
    
    // 特定ボタン用の色を取得するメソッド
    juce::Colour getSpecialButtonColour(ButtonType type) const {
        switch (type) {
            case SAVE:
                return saveButtonColour;
            case COMPILE:
                return compileButtonColour;
            default:
                return juce::Colour(0x69, 0x69, 0x69); // デフォルト色
        }
    }
    
    // テキストボタンの背景描画
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                             const juce::Colour& backgroundColour,
                             bool shouldDrawButtonAsHighlighted,
                             bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);
        auto cornerSize = 4.0f;
        
        // 特殊ボタンの識別
        ButtonType buttonType = NORMAL;
        if (button.getName() == "Save") buttonType = SAVE;
        else if (button.getName() == "Compile") buttonType = COMPILE;
        
        // 選択状態に基づいて塗りつぶし色を決定
        juce::Colour fillColour;
        
        if (button.getToggleState()) {
            fillColour = juce::Colour(0xd9, 0xd9, 0xd9); // 選択時: 白背景(#d9d9d9)
        } else {
            // 特殊ボタンの場合は専用色を使用
            if (buttonType != NORMAL) {
                fillColour = getSpecialButtonColour(buttonType);
            } else {
                fillColour = juce::Colour(0x45, 0x45, 0x45); // 非選択時: 暗めのグレー背景(#454545)
            }
        }
        
        // 押下時の効果（インナーシャドウ）
        if (shouldDrawButtonAsDown) {
            // ボタンが押されているときは影を内側に
            g.setColour(fillColour.darker(0.2f));
            g.fillRoundedRectangle(bounds, cornerSize);
            
            // 内側の影効果
            g.setColour(juce::Colours::black.withAlpha(0.4f));
            g.drawRoundedRectangle(bounds.reduced(0.5f), cornerSize, 1.5f);
            
            // メインの塗りつぶし（少しサイズを小さく、位置を少しずらす）
            auto innerBounds = bounds.reduced(2.0f);
            g.setColour(fillColour);
            g.fillRoundedRectangle(innerBounds, cornerSize - 1.0f);
        } else {
            // 通常表示時（ドロップシャドウ効果）
            // 影を描画
            g.setColour(juce::Colours::black.withAlpha(0.2f));
            auto shadowBounds = bounds.translated(2.0f, 2.0f);
            g.fillRoundedRectangle(shadowBounds, cornerSize);
            
            // メインの塗りつぶし
            g.setColour(fillColour);
            g.fillRoundedRectangle(bounds, cornerSize);
            
            // ハイライト時は明るめの枠線
            if (shouldDrawButtonAsHighlighted) {
                g.setColour(fillColour.brighter(0.2f));
                g.drawRoundedRectangle(bounds, cornerSize, 1.5f);
            }
        }
        
        // 非選択時は白い枠線を描画（通常時のみ）
        if (!button.getToggleState() && !shouldDrawButtonAsDown) {
            g.setColour(juce::Colours::white.withAlpha(0.7f));
            g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
        }
    }
    
    // テキストボタンのテキスト描画
    void drawButtonText(juce::Graphics& g, juce::TextButton& button,
                       bool shouldDrawButtonAsHighlighted,
                       bool shouldDrawButtonAsDown) override
    {
        // 固定フォントサイズを使用
        g.setFont(juce::Font(15.0f));
        
        // ボタンの状態に応じたテキスト色を設定
        if (button.getToggleState()) {
            g.setColour(juce::Colour(0x45, 0x45, 0x45)); // 選択時: グレーテキスト
        } else {
            g.setColour(juce::Colours::white); // 非選択時: 白テキスト
        }
        
        // 押下時は少しテキスト位置をずらす
        auto textBounds = button.getLocalBounds();
        if (shouldDrawButtonAsDown) {
            textBounds.translate(1, 1); // テキストを少し右下にずらす
        }
        
        // テキストを中央揃えで描画
        g.drawText(button.getButtonText(), textBounds, juce::Justification::centred, true);
    }
    
    // トグルボタンの描画
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                         bool shouldDrawButtonAsHighlighted,
                         bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);
        auto cornerSize = 4.0f;
        
        // 選択状態に基づいて塗りつぶし色を決定
        juce::Colour fillColour;
        
        if (button.getToggleState()) {
            fillColour = juce::Colour(0xd9, 0xd9, 0xd9); // 選択時: 白背景(#d9d9d9)
        } else {
            fillColour = juce::Colour(0x45, 0x45, 0x45); // 非選択時: 暗めのグレー背景(#454545)
        }
        
        // 押下時の効果（インナーシャドウ）
        if (shouldDrawButtonAsDown) {
            // ボタンが押されているときは影を内側に
            g.setColour(fillColour.darker(0.2f));
            g.fillRoundedRectangle(bounds, cornerSize);
            
            // 内側の影効果
            g.setColour(juce::Colours::black.withAlpha(0.4f));
            g.drawRoundedRectangle(bounds.reduced(0.5f), cornerSize, 1.5f);
            
            // メインの塗りつぶし（少しサイズを小さく、位置を少しずらす）
            auto innerBounds = bounds.reduced(2.0f);
            g.setColour(fillColour);
            g.fillRoundedRectangle(innerBounds, cornerSize - 1.0f);
        } else {
            // 通常表示時（ドロップシャドウ効果）
            // 影を描画
            g.setColour(juce::Colours::black.withAlpha(0.2f));
            auto shadowBounds = bounds.translated(2.0f, 2.0f);
            g.fillRoundedRectangle(shadowBounds, cornerSize);
            
            // メインの塗りつぶし
            g.setColour(fillColour);
            g.fillRoundedRectangle(bounds, cornerSize);
            
            // ハイライト時は明るめの枠線
            if (shouldDrawButtonAsHighlighted) {
                g.setColour(fillColour.brighter(0.2f));
                g.drawRoundedRectangle(bounds, cornerSize, 1.5f);
            }
        }
        
        // 非選択時は白い枠線を描画（通常時のみ）
        if (!button.getToggleState() && !shouldDrawButtonAsDown) {
            g.setColour(juce::Colours::white.withAlpha(0.7f));
            g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
        }
        
        // 固定フォントサイズを使用
        g.setFont(juce::Font(15.0f));
        
        // ボタンの状態に応じたテキスト色を設定
        if (button.getToggleState()) {
            g.setColour(juce::Colour(0x45, 0x45, 0x45)); // 選択時: グレーテキスト
        } else {
            g.setColour(juce::Colours::white); // 非選択時: 白テキスト
        }
        
        // 押下時は少しテキスト位置をずらす
        auto textBounds = button.getLocalBounds();
        if (shouldDrawButtonAsDown) {
            textBounds.translate(1, 1); // テキストを少し右下にずらす
        }
        
        // テキストを中央揃えで描画
        g.drawText(button.getButtonText(), textBounds, juce::Justification::centred, true);
    }
    
private:
    juce::Colour saveButtonColour;
    juce::Colour compileButtonColour;
};
