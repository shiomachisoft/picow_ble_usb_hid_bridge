# Verified devices

Devices someone has actually run the bridge with, and the commit they ran.
Firmware this low-level breaks in device-specific ways, so an entry is only
meaningful together with the commit it was tested at.

| Device                 | Type     | Board    | Commit    |
|------------------------|----------|----------|-----------|
| Sanwa Supply MA-SBB314 | Mouse    | Pico W   | `399c6b2` |
| ELECOM TK-FBM119       | Keyboard | Pico W   | `399c6b2` |
| ASUS KW100             | Keyboard | Pico 2 W | `64810f7` |

The first two entries predate Pico 2 W support and were verified upstream, at
the commit this work branched from.

To add a device, run `git rev-parse --short HEAD` in the tree you flashed from
and add a row with that hash.
