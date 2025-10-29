package com.theengs.bridge;

import android.app.Service;
import android.bluetooth.*;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;
import java.io.*;
import java.nio.charset.StandardCharsets;

public class BleService extends Service {
    private BluetoothLeScanner scanner;
    private LocalServerSocket serverSocket;

    public void onCreate() {
        scanner = ((BluetoothManager)getSystemService(BLUETOOTH_SERVICE))
                   .getAdapter().getBluetoothLeScanner();
        try {
            serverSocket = new LocalServerSocket("theengs/ble");
            new Thread(this::acceptLoop).start();
        } catch (IOException e) { stopSelf(); }
    }

    private void acceptLoop() {
        while (true) {
            try (LocalSocket client = serverSocket.accept();
                 OutputStream out = client.getOutputStream()) {
                synchronized (clients) { clients.add(out); }
            } catch (IOException ignore) {}
        }
    }

    private final List<OutputStream> clients = new ArrayList<>();

    private final ScanCallback callback = new ScanCallback() {
        public void onScanResult(int t, ScanResult r) {
            byte[] raw = r.getScanRecord().getBytes();
            String json = decode(raw, r.getRssi(), r.getDevice().getAddress());
            if (json == null) return;
            synchronized (clients) {
                for (OutputStream o : clients) {
                    try { o.write((json + "\n").getBytes(StandardCharsets.UTF_8)); }
                    catch (IOException ignore) {}
                }
            }
        }
    };

    private native String decode(byte[] adv, int rssi, String mac);
    static { System.loadLibrary("theengs-jni"); }

    public int onStartCommand(Intent i, int flags, int startId) {
        scanner.startScan(new ArrayList<>(), new ScanSettings.Builder()
                          .setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY).build(), callback);
        return START_STICKY;
    }
    public void onDestroy() { scanner.stopScan(callback); }
    public IBinder onBind(Intent i) { return null; }
}
