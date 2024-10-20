#include <settings.h>

#include <QSignalSpy>
#include <QTest>

class SettingsTest : public QObject {
  Q_OBJECT

 private slots:
  void testDefaultValues() const;
  void testLanguage();
  void testWindowOpacity();
  void testFpsVisible();
  void testShortcuts();
  void testResetShortcutsToDefaults();

 private:
  llm_chat::Settings m_Settings;
};

void SettingsTest::testDefaultValues() const {
  QCOMPARE(m_Settings.defaultLanguage(), QString("en_GB"));
  QCOMPARE(m_Settings.defaultWindowOpacity(), 1.0);
  QCOMPARE(m_Settings.defaultFpsVisible(), false);
  QCOMPARE(m_Settings.defaultQuitShortcut(), QString("Ctrl+Q"));
  QCOMPARE(m_Settings.defaultOptionsShortcut(), QString("Ctrl+O"));
  QCOMPARE(m_Settings.defaultFullScreenShortcut(), QString("Ctrl+F"));
}

void SettingsTest::testLanguage() {
  m_Settings.setLanguage("fr");
  QCOMPARE(m_Settings.language(), QString("fr"));
  QSignalSpy spy(&m_Settings, &llm_chat::Settings::languageChanged);
  m_Settings.setLanguage("de");
  QCOMPARE(spy.count(), 1);
  QCOMPARE(m_Settings.language(), QString("de"));
}

void SettingsTest::testWindowOpacity() {
  m_Settings.setWindowOpacity(0.8);
  QCOMPARE(m_Settings.windowOpacity(), 0.8);
  QSignalSpy spy(&m_Settings, &llm_chat::Settings::windowOpacityChanged);
  m_Settings.setWindowOpacity(0.5);
  QCOMPARE(spy.count(), 1);
  QCOMPARE(m_Settings.windowOpacity(), 0.5);
}

void SettingsTest::testFpsVisible() {
  m_Settings.setFpsVisible(true);
  QCOMPARE(m_Settings.isFpsVisible(), true);
  QSignalSpy spy(&m_Settings, &llm_chat::Settings::fpsVisibleChanged);
  m_Settings.setFpsVisible(false);
  QCOMPARE(spy.count(), 1);
  QCOMPARE(m_Settings.isFpsVisible(), false);
}

void SettingsTest::testShortcuts() {
  m_Settings.setQuitShortcut("Ctrl+X");
  QCOMPARE(m_Settings.quitShortcut(), QString("Ctrl+X"));
  QSignalSpy quitSpy(&m_Settings, &llm_chat::Settings::quitShortcutChanged);
  m_Settings.setQuitShortcut("Ctrl+Q");
  QCOMPARE(quitSpy.count(), 1);
  QCOMPARE(m_Settings.quitShortcut(), QString("Ctrl+Q"));

  m_Settings.setOptionsShortcut("Ctrl+P");
  QCOMPARE(m_Settings.optionsShortcut(), QString("Ctrl+P"));
  QSignalSpy optionsSpy(&m_Settings,
                        &llm_chat::Settings::optionsShortcutChanged);
  m_Settings.setOptionsShortcut("Ctrl+O");
  QCOMPARE(optionsSpy.count(), 1);
  QCOMPARE(m_Settings.optionsShortcut(), QString("Ctrl+O"));

  m_Settings.setFullScreenShortcut("Ctrl+G");
  QCOMPARE(m_Settings.fullScreenShortcut(), QString("Ctrl+G"));
  QSignalSpy fullScreenSpy(&m_Settings,
                           &llm_chat::Settings::fullScreenShortcutChanged);
  m_Settings.setFullScreenShortcut("Ctrl+F");
  QCOMPARE(fullScreenSpy.count(), 1);
  QCOMPARE(m_Settings.fullScreenShortcut(), QString("Ctrl+F"));
}

void SettingsTest::testResetShortcutsToDefaults() {
  m_Settings.setQuitShortcut("Ctrl+X");
  m_Settings.setOptionsShortcut("Ctrl+P");
  m_Settings.setFullScreenShortcut("Ctrl+G");
  m_Settings.resetShortcutsToDefaults();
  QCOMPARE(m_Settings.quitShortcut(), m_Settings.defaultQuitShortcut());
  QCOMPARE(m_Settings.optionsShortcut(), m_Settings.defaultOptionsShortcut());
  QCOMPARE(m_Settings.fullScreenShortcut(),
           m_Settings.defaultFullScreenShortcut());
}

QTEST_MAIN(SettingsTest)
#include "tst_settings.moc"