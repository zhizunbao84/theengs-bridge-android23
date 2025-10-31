package com.theengs.bridge;


import android.app.Service;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.bluetooth.le.ScanSettings;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.LocalServerSocket;
import android.net.LocalSocket;
import android.os.IBinder;
import android.util.Log;

import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

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
