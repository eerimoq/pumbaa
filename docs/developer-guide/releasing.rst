Releasing
=========

Follow these steps to create a new release:

1. Write the new version in ``VERSION.txt``. The version should hace
   the format ``<major>.<minor>.<revision>``.

   Increment ``<major>`` for non-backwards compatible changes.

   Increment ``<minor>`` for new features.

   Increment ``<revision>`` for bug fixes.

2. Run the test suites and generate the documentation.

   .. code:: text

      make test

3. Generate files for Arduino.

   .. code:: text

      make arduino

4. Add the new releases to
   ``make/arduino/<family>/package_pumbaa_<family>_index.json``. The
   sha256 sums of the zip-archives are calculated by ``make arduino``
   and written to ``pumbaa-arduino/*.sha256``.

5. Copy the Pumbaa Arduino releases to the release repository, add,
   commit and push in the release repository.

   .. code:: text

      cp pumbaa-arduino/pumbaa-arduino-sam-*.zip ../pumbaa-releases/arduino/sam
      cp pumbaa-arduino/pumbaa-arduino-esp32-*.zip ../pumbaa-releases/arduino/esp32

6. Start a http server used to download package manifests in the Arduino IDE.

   .. code:: text

      (cd make/arduino && python -m SimpleHTTPServer)

7. Start the Arduino IDE and add these URL:s in Preferences.

   .. code:: text

      http://localhost:8000/esp32/package_pumbaa_esp32_index.json
      http://localhost:8000/sam/package_pumbaa_sam_index.json

8. Install all packages and run the blink example for each one of
   them.

9. Commit the changes, and tag the commit with the new version.

10. Push the new commit and tag.

11. Copy the Pumbaa Arduino package manifests to the release
    repository, add, commit and push in the release repository.

   .. code:: text

      cp make/arduino/sam/package_pumbaa_sam_index.json ../pumbaa-releases/arduino/sam
      cp make/arduino/esp32/package_pumbaa_esp32_index.json ../pumbaa-releases/arduino/esp32

12. Done.
