package com.platymuus.hamsandwich;

import android.view.View;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.util.ArrayList;

public class Game {
	public final String id;
	public final String title;
	public final boolean excluded;

	public final ArrayList<Asset> assets = new ArrayList<>();

	public View page;

	public Game(File installersDir, String id, JSONObject project) throws JSONException {
		this.id = id;
		title = project.getString("title");
		excluded = project.has("excluded") && project.getBoolean("excluded");
		// TODO: icon
		JSONArray installers = project.getJSONArray("installers");
		for (int j = 0; j < installers.length(); ++j) {
			assets.add(new Asset(installersDir, installers.getJSONObject(j)));
		}
	}
	// TODO: icon

	public boolean startMissingDownloads(UiThreadHandle uiThreadHandle) {
		boolean anyMissing = false;
		for (Asset asset : assets) {
			if (asset.required || asset.checkbox.isChecked()) {
				if (!asset.file.exists()) {
					asset.startDownload(uiThreadHandle);
					anyMissing = true;
				}
			}
		}
		return anyMissing;
	}

	public boolean isReadyToPlay() {
		for (Asset asset : assets) {
			if (asset.required || asset.checkbox.isChecked()) {
				if (!asset.file.exists()) {
					return false;
				}
			}
		}
		return true;
	}
}
