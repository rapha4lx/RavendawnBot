#include <iostream>

#include "Recognition.h"

#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <opencv2/dnn/dnn.hpp>



bool Recognition::testefishi(HWND& hWnd, ImageType imageType, double threshold, bool debug) {
	try {
		RECT rect;
		GetClientRect(hWnd, &rect);
		int width = rect.right;
		int height = rect.bottom;

		HDC hdcScreen = GetDC(hWnd);
		if (!hdcScreen)
			return false;

		HDC hdcMem = CreateCompatibleDC(hdcScreen);
		if (!hdcMem) {
			ReleaseDC(hWnd, hdcScreen);
			return false;
		}

		HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
		if (!hBitmap) {
			ReleaseDC(hWnd, hdcScreen);
			DeleteDC(hdcMem);
			return false;
		}

		SelectObject(hdcMem, hBitmap);

		if (!BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY)) {
			ReleaseDC(hWnd, hdcScreen);
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			return false;
		}

		ReleaseDC(hWnd, hdcScreen);
		DeleteDC(hdcMem);

		cv::Mat screenShot(height, width, CV_8UC3);
		if (screenShot.empty()) {
			DeleteObject(hBitmap);
			return false;
		}

		BITMAPINFOHEADER bi;
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = width;
		bi.biHeight = -height;
		bi.biPlanes = 1;
		bi.biBitCount = 24;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		GetDIBits(hdcScreen, hBitmap, 0, height, screenShot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

		DeleteObject(hBitmap);

		cv::resize(screenShot, screenShot, cv::Size(screenShot.cols / 2, screenShot.rows / 2)); // Reduz a resolução pela metade

		for (auto& image : Images) {
			if (image.second != imageType)
				continue;

			cv::Mat subImage = cv::imread(image.first);
			if (subImage.empty())
				continue;

			cv::Mat resultImage;
			cv::matchTemplate(screenShot, subImage, resultImage, cv::TM_CCOEFF_NORMED);

			cv::Mat thresholdedImage;
			cv::threshold(resultImage, thresholdedImage, threshold, 1.0, cv::THRESH_TOZERO);

			std::vector<cv::Point> locations;
			cv::findNonZero(thresholdedImage, locations);

			if (!locations.empty()) {
				if (debug) {
					for (const auto& loc : locations) {
						cv::rectangle(screenShot, loc, cv::Point(loc.x + subImage.cols, loc.y + subImage.rows), cv::Scalar(0, 255, 0), 2);
						int centerX = loc.x + subImage.cols / 2;
						int centerY = loc.y + subImage.rows / 2;
						cv::Point center(centerX, centerY);
						cv::circle(screenShot, center, 5, cv::Scalar(0, 0, 255), -1);
					}
				}

				if (debug) {
					cv::namedWindow("Debug", cv::WINDOW_NORMAL);
					cv::imshow("Debug", screenShot);
					if (cv::waitKey(1) == 'q') {
						screenShot.release();
						subImage.release();
						resultImage.release();
						thresholdedImage.release();
						return false;
					}
				}

				return true; // Encontrou padrão
			}

			subImage.release();
			resultImage.release();
			thresholdedImage.release();
		}

		screenShot.release();
		return false; // Não encontrou padrão
	}
	catch (const std::exception& e) {
		std::cerr << "Exceção capturada: " << e.what() << std::endl;
	}
	catch (const cv::Exception& e) {
		std::cerr << "Exceção capturada: " << e.what() << std::endl;
	}
	return false;
}


bool Recognition::CustomRecognition(HWND& hWnd, ImageType imageType, double threshold, bool debug) {
	try
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		int width = rect.right;
		int height = rect.bottom;

		HDC hdcScreen;
		try
		{
			hdcScreen = GetDC(hWnd);
			if (!hdcScreen) {

				return false;
			}
		}
		catch (const std::exception&)
		{

			return false;
		}


		HDC hdcMem;
		try
		{
			hdcMem = CreateCompatibleDC(hdcScreen);
			if (!hdcMem) {
				ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
				return false;
			}
		}
		catch (const std::exception&)
		{
			ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
			return false;
		}

		HBITMAP hBitmap;
		try
		{
			hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
			if (!hBitmap) {
				ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
				DeleteDC(hdcMem);
				return false;
			}
		}
		catch (const std::exception&)
		{
			ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
			DeleteDC(hdcMem);
			return false;
		}

		SelectObject(hdcMem, hBitmap);

		if (!BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY)) {
			ReleaseDC(hWnd, hdcScreen);
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			return false;
		}

		BITMAPINFOHEADER bi;
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = width;
		bi.biHeight = -height;
		bi.biPlanes = 1;
		bi.biBitCount = 24;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		cv::Mat screenShot;
		try
		{
			screenShot.create(height, width, CV_8UC3);
			if (screenShot.empty()) {
				ReleaseDC(hWnd, hdcScreen);
				DeleteDC(hdcMem);
				DeleteObject(hBitmap);
				screenShot.release();
				return false;
			}
		}
		catch (const cv::Exception& e)
		{
			std::cout << e.what() << std::endl;
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			ReleaseDC(hWnd, hdcScreen);
			screenShot.release();
			return false;
		}

		if (!GetDIBits(hdcScreen, hBitmap, 0, height, screenShot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS)) {
			ReleaseDC(hWnd, hdcScreen);
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			screenShot.release();
			return false;
		}

		// Liberar recursos
		ReleaseDC(hWnd, hdcScreen);
		DeleteDC(hdcMem);
		DeleteObject(hBitmap);

		std::vector<cv::Point> locations;
		for (auto& image : Images) {
			if (image.second != imageType) {
				continue;
			}

			cv::Mat subImage = cv::imread(image.first);

			if (subImage.empty()) {
				subImage.release();
				continue;
			}

			int resultCols = screenShot.cols - subImage.cols + 1;
			int resultRows = screenShot.rows - subImage.rows + 1;
			cv::Mat resultImage;
			try
			{
				resultImage.create(resultRows, resultCols, CV_32FC1);
			}
			catch (const cv::Exception&)
			{
				screenShot.release();
				subImage.release();
				resultImage.release();
				continue;
			}

			cv::matchTemplate(screenShot, subImage, resultImage, cv::TM_CCOEFF_NORMED);

			cv::Mat thresholdedImage;
			cv::threshold(resultImage, thresholdedImage, threshold, 1.0, cv::THRESH_TOZERO);

			cv::findNonZero(thresholdedImage, locations);

			if (locations.size() > 0) {
				if (debug) {
					for (const auto& loc : locations) {
						cv::rectangle(screenShot, loc, cv::Point(loc.x + subImage.cols, loc.y + subImage.rows), cv::Scalar(0, 255, 0), 2);

						int centerX = loc.x + subImage.cols / 2;
						int centerY = loc.y + subImage.rows / 2;
						cv::Point center(centerX, centerY);
						cv::circle(screenShot, center, 5, cv::Scalar(0, 0, 255), -1);
						//cv::line(screenShot, center, cv::Point(center.x + pos.x, center.y + pos.y), cv::Scalar(255, 0, 0), 2);

					}
				}

				if (debug) {
					try
					{
						cv::namedWindow("Debug", cv::WINDOW_NORMAL);
						cv::imshow("Debug", screenShot);

						// Pressione 'q' para sair do loop
						if (cv::waitKey(1) == 'q') {
							screenShot.release();
							subImage.release();
							resultImage.release();
							thresholdedImage.release();
							return false;
						}

					}
					catch (const cv::Exception& e)
					{
						screenShot.release();
						subImage.release();
						resultImage.release();
						thresholdedImage.release();
						std::cerr << "Exceção capturada: " << e.what() << std::endl;
					}
				}
				screenShot.release();
				subImage.release();
				resultImage.release();
				thresholdedImage.release();
				return true;
			}
		}
		screenShot.release();
		return false;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exceção capturada: " << e.what() << std::endl;

	}
	catch (const cv::Exception& e) {
		std::cerr << "Exceção capturada: " << e.what() << std::endl;

	}
	return false;
}

bool Recognition::CustomRecognition(HWND& hWnd, ImageType imageType, double threshold, POINT& pos, bool debug) {
	try
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		int width = rect.right;
		int height = rect.bottom;

		HDC hdcScreen;
		try
		{
			hdcScreen = GetDC(hWnd);
			if (!hdcScreen) {

				return false;
			}
		}
		catch (const std::exception&)
		{

			return false;
		}


		HDC hdcMem;
		try
		{
			hdcMem = CreateCompatibleDC(hdcScreen);
			if (!hdcMem) {
				ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
				return false;
			}
		}
		catch (const std::exception&)
		{
			ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
			return false;
		}

		HBITMAP hBitmap;
		try
		{
			hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
			if (!hBitmap) {
				ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
				DeleteDC(hdcMem);
				return false;
			}
		}
		catch (const std::exception&)
		{
			ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
			DeleteDC(hdcMem);
			return false;
		}

		SelectObject(hdcMem, hBitmap);

		if (!BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY)) {
			ReleaseDC(hWnd, hdcScreen);
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			return false;
		}

		BITMAPINFOHEADER bi;
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = width;
		bi.biHeight = -height;
		bi.biPlanes = 1;
		bi.biBitCount = 24;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		cv::Mat screenShot;
		try
		{
			screenShot.create(height, width, CV_8UC3);
			if (screenShot.empty()) {
				ReleaseDC(hWnd, hdcScreen);
				DeleteDC(hdcMem);
				DeleteObject(hBitmap);
				screenShot.release();
				return false;
			}
		}
		catch (const cv::Exception& e)
		{
			std::cout << e.what() << std::endl;
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			ReleaseDC(hWnd, hdcScreen);
			screenShot.release();
			return false;
		}

		if (!GetDIBits(hdcScreen, hBitmap, 0, height, screenShot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS)) {
			ReleaseDC(hWnd, hdcScreen);
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			return false;
		}

		// Liberar recursos
		ReleaseDC(hWnd, hdcScreen);
		DeleteDC(hdcMem);
		DeleteObject(hBitmap);

		std::vector<cv::Point> locations;
		for (auto& image : Images) {
			if (image.second != imageType) {
				continue;
			}

			cv::Mat subImage = cv::imread(image.first);

			if (subImage.empty()) {
				subImage.release();
				continue;
			}

			int resultCols = screenShot.cols - subImage.cols + 1;
			int resultRows = screenShot.rows - subImage.rows + 1;
			cv::Mat resultImage;
			try
			{
				resultImage = cv::Mat(resultRows, resultCols, CV_32FC1);
			}
			catch (const cv::Exception&)
			{
				subImage.release();
				resultImage.release();
				continue;
			}

			cv::matchTemplate(screenShot, subImage, resultImage, cv::TM_CCOEFF_NORMED);

			cv::Mat thresholdedImage;
			cv::threshold(resultImage, thresholdedImage, threshold, 1.0, cv::THRESH_TOZERO);

			cv::findNonZero(thresholdedImage, locations);

			if (locations.size() > 0) {
				if (debug) {
					for (const auto& loc : locations) {
						cv::rectangle(screenShot, loc, cv::Point(loc.x + subImage.cols, loc.y + subImage.rows), cv::Scalar(0, 255, 0), 2);

						int centerX = loc.x + subImage.cols / 2;
						int centerY = loc.y + subImage.rows / 2;
						cv::Point center(centerX, centerY);
						cv::circle(screenShot, center, 5, cv::Scalar(0, 0, 255), -1);
						cv::line(screenShot, center, cv::Point(center.x + pos.x, center.y + pos.y), cv::Scalar(255, 0, 0), 2);

					}
				}

				if (debug) {
					try
					{
						cv::namedWindow("Debug", cv::WINDOW_NORMAL);
						cv::imshow("Debug", screenShot);

						// Pressione 'q' para sair do loop
						if (cv::waitKey(1) == 'q') {
							screenShot.release();
							subImage.release();
							resultImage.release();
							thresholdedImage.release();
							return false;
						}

					}
					catch (const cv::Exception& e)
					{
						std::cerr << "Exceção capturada: " << e.what() << std::endl;
						screenShot.release();
						subImage.release();
						resultImage.release();
						thresholdedImage.release();
						return false;
					}
				}

				int centerX = locations[0].x + subImage.cols / 2;
				int centerY = locations[0].y + subImage.rows / 2;
				cv::Point center(centerX, centerY);

				pos.x += center.x;
				pos.y += center.y;

				screenShot.release();
				subImage.release();
				resultImage.release();
				thresholdedImage.release();
				return true;
			}
		}
		screenShot.release();
		return false;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exceção capturada: " << e.what() << std::endl;

	}
	catch (const cv::Exception& e) {
		std::cerr << "Exceção capturada: " << e.what() << std::endl;

	}
	return false;
}

bool Recognition::CustomRecognition(HWND& hWnd, int posX, int posY, int sizeX, int sizeY, ImageType imageType, double threshold) {
	try
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		int width = rect.right;
		int height = rect.bottom;

		HDC hdcScreen;
		try
		{
			hdcScreen = GetDC(hWnd);
			if (!hdcScreen) {

				return false;
			}
		}
		catch (const std::exception&)
		{

			return false;
		}


		HDC hdcMem;
		try
		{
			hdcMem = CreateCompatibleDC(hdcScreen);
			if (!hdcMem) {
				ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
				return false;
			}
		}
		catch (const std::exception&)
		{
			ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
			return false;
		}

		HBITMAP hBitmap;
		try
		{
			hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
			if (!hBitmap) {
				ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
				DeleteDC(hdcMem);
				return false;
			}
		}
		catch (const std::exception&)
		{
			ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
			DeleteDC(hdcMem);
			return false;
		}

		SelectObject(hdcMem, hBitmap);

		if (!BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY)) {
			ReleaseDC(hWnd, hdcScreen);
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			return false;
		}

		BITMAPINFOHEADER bi;
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = width;
		bi.biHeight = -height;
		bi.biPlanes = 1;
		bi.biBitCount = 24;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		cv::Mat screenShot;
		try
		{
			screenShot.create(height, width, CV_8UC3);
			if (screenShot.empty()) {
				ReleaseDC(hWnd, hdcScreen);
				DeleteDC(hdcMem);
				DeleteObject(hBitmap);
				screenShot.release();
				return false;
			}
		}
		catch (const cv::Exception& e)
		{
			std::cout << e.what() << std::endl;
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			ReleaseDC(hWnd, hdcScreen);
			screenShot.release();
			return false;
		}

		if (!GetDIBits(hdcScreen, hBitmap, 0, height, screenShot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS)) {
			ReleaseDC(hWnd, hdcScreen);
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			screenShot.release();
			return false;
		}

		// Liberar recursos
		ReleaseDC(hWnd, hdcScreen);
		DeleteDC(hdcMem);
		DeleteObject(hBitmap);

		posX = std::max(0, std::min(posX, width - 1));
		posY = std::max(0, std::min(posY, height - 1));
		sizeX = std::min(sizeX, width - posX);
		sizeY = std::min(sizeY, height - posY);

		// Cortar a área específica
		cv::Rect roi(posX, posY, sizeX, sizeY);
		cv::Mat croppedImage = screenShot(roi).clone();  // Clone para garantir uma cópia independente

		std::vector<cv::Point> locations;
		for (auto& image : Images) {
			if (image.second != imageType) {
				continue;
			}

			cv::Mat subImage = cv::imread(image.first);

			if (subImage.empty()) {
				subImage.release();
				continue;
			}

			int resultCols = croppedImage.cols - subImage.cols + 1;
			int resultRows = croppedImage.rows - subImage.rows + 1;
			cv::Mat resultImage;
			try
			{
				resultImage.create(resultRows, resultCols, CV_32FC1);
			}
			catch (const cv::Exception&)
			{
				resultImage.release();
				subImage.release();
				continue;
			}

			cv::matchTemplate(croppedImage, subImage, resultImage, cv::TM_CCOEFF_NORMED);

			cv::Mat thresholdedImage;
			cv::threshold(resultImage, thresholdedImage, threshold, 1.0, cv::THRESH_TOZERO);

			cv::findNonZero(thresholdedImage, locations);

			if (locations.size() > 0) {
				//try
				//{
				//	cv::namedWindow("Debug", cv::WINDOW_NORMAL);
				//	cv::imshow("Debug", croppedImage);
				//	// Pressione 'q' para sair do loop
				//	if (cv::waitKey(0) == 'q') {
				//		DeleteDC(hdcMem);
				//		DeleteObject(hBitmap);
				//		ReleaseDC(hWnd, hdcScreen);
				//		return false;
				//	}
				//}
				//catch (const cv::Exception& e)
				//{
				//	std::cerr << "Exceção capturada: " << e.what() << std::endl;
				//	DeleteDC(hdcMem);
				//	DeleteObject(hBitmap);
				//	ReleaseDC(hWnd, hdcScreen);
				//}
				croppedImage.release();
				thresholdedImage.release();
				subImage.release();
				resultImage.release();
				return true;
			}

			//try
			//{
			//	cv::namedWindow("Debug", cv::WINDOW_NORMAL);
			//	cv::imshow("Debug", croppedImage);
			//	// Pressione 'q' para sair do loop
			//	if (cv::waitKey(0) == 'q') {
			//		DeleteDC(hdcMem);
			//		DeleteObject(hBitmap);
			//		ReleaseDC(hWnd, hdcScreen);
			//		return false;
			//	}
			//}
			//catch (const cv::Exception& e)
			//{
			//	std::cerr << "Exceção capturada: " << e.what() << std::endl;
			//	DeleteDC(hdcMem);
			//	DeleteObject(hBitmap);
			//	ReleaseDC(hWnd, hdcScreen);
			//}
			resultImage.release();
			thresholdedImage.release();
			subImage.release();
		}
		screenShot.release();
		croppedImage.release();
		return false;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exceção capturada: " << e.what() << std::endl;

	}
	catch (const cv::Exception& e) {
		std::cerr << "Exceção capturada: " << e.what() << std::endl;

	}
	return false;
}

bool Recognition::CustomRecognition(HWND& hWnd, int posX, int posY, int sizeX, int sizeY, ImageType imageType, double threshold, POINT& pos) {
	try
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		int width = rect.right;
		int height = rect.bottom;

		HDC hdcScreen;
		try
		{
			hdcScreen = GetDC(hWnd);
			if (!hdcScreen) {

				return false;
			}
		}
		catch (const std::exception&)
		{

			return false;
		}


		HDC hdcMem;
		try
		{
			hdcMem = CreateCompatibleDC(hdcScreen);
			if (!hdcMem) {
				ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
				return false;
			}
		}
		catch (const std::exception&)
		{
			ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
			return false;
		}

		HBITMAP hBitmap;
		try
		{
			hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
			if (!hBitmap) {
				ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
				DeleteDC(hdcMem);
				return false;
			}
		}
		catch (const std::exception&)
		{
			ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
			DeleteDC(hdcMem);
			return false;
		}

		SelectObject(hdcMem, hBitmap);

		if (!BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY)) {
			ReleaseDC(hWnd, hdcScreen);
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			return false;
		}

		BITMAPINFOHEADER bi;
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = width;
		bi.biHeight = -height;
		bi.biPlanes = 1;
		bi.biBitCount = 24;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		cv::Mat screenShot;
		try
		{
			screenShot.create(height, width, CV_8UC3);
			if (screenShot.empty()) {
				ReleaseDC(hWnd, hdcScreen);
				DeleteDC(hdcMem);
				DeleteObject(hBitmap);
				screenShot.release();
				return false;
			}
		}
		catch (const cv::Exception& e)
		{
			std::cout << e.what() << std::endl;
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			ReleaseDC(hWnd, hdcScreen);
			screenShot.release();
			return false;
		}

		if (!GetDIBits(hdcScreen, hBitmap, 0, height, screenShot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS)) {
			ReleaseDC(hWnd, hdcScreen);
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			screenShot.release();
			return false;
		}

		// Liberar recursos
		ReleaseDC(hWnd, hdcScreen);
		DeleteDC(hdcMem);
		DeleteObject(hBitmap);

		posX = std::max(0, std::min(posX, width - 1));
		posY = std::max(0, std::min(posY, height - 1));
		sizeX = std::min(sizeX, width - posX);
		sizeY = std::min(sizeY, height - posY);

		// Cortar a área específica
		cv::Rect roi(posX, posY, sizeX, sizeY);
		cv::Mat croppedImage = screenShot(roi).clone();  // Clone para garantir uma cópia independente

		for (auto& image : Images) {
			if (image.second != imageType) {
				continue;
			}

			cv::Mat subImage = cv::imread(image.first);

			if (subImage.empty()) {
				subImage.release();
				continue;
			}

			int resultCols = croppedImage.cols - subImage.cols + 1;
			int resultRows = croppedImage.rows - subImage.rows + 1;
			cv::Mat resultImage;
			try
			{
				resultImage = cv::Mat(resultRows, resultCols, CV_32FC1);
			}
			catch (const cv::Exception&)
			{
				croppedImage.release();
				subImage.release();
				resultImage.release();
				break;
			}

			cv::matchTemplate(croppedImage, subImage, resultImage, cv::TM_CCOEFF_NORMED);

			cv::Mat thresholdedImage;
			cv::threshold(resultImage, thresholdedImage, threshold, 1.0, cv::THRESH_TOZERO);

			std::vector<cv::Point> locations;
			cv::findNonZero(thresholdedImage, locations);

			if (locations.size() > 0) {
				for (const auto& loc : locations) {
					cv::rectangle(screenShot, loc, cv::Point(loc.x + subImage.cols, loc.y + subImage.rows), cv::Scalar(0, 255, 0), 2);
					//cv::putText(screenShot, loc.x + ":" + loc.y, cv::Point(loc.x + subImage.cols, loc.y + subImage.rows), 1, 1, cv::Scalar(255, 0, 0, 1));
				}
			}
			thresholdedImage.release();
			subImage.release();
			resultImage.release();
		}


		cv::namedWindow("Debug", cv::WINDOW_NORMAL);
		cv::imshow("Debug", screenShot);

		// Pressione 'q' para sair do loop
		if (cv::waitKey(0) == 'q') {
			croppedImage.release();
			screenShot.release();
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			ReleaseDC(hWnd, hdcScreen);
			return false;
		}

		screenShot.release();
		croppedImage.release();
		DeleteDC(hdcMem);
		DeleteObject(hBitmap);
		ReleaseDC(hWnd, hdcScreen);
		return false;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exceção capturada: " << e.what() << std::endl;

	}
	catch (const cv::Exception& e) {
		std::cerr << "Exceção capturada: " << e.what() << std::endl;

	}
	return false;
}

void Recognition::UpdatePos(HWND& hWnd, cv::Mat& player) {
	POINT pos{};
	RECT rect;
	GetClientRect(hWnd, &rect);
	int width = rect.right;
	int height = rect.bottom;

	HDC hdcScreen;
	try
	{
		hdcScreen = GetDC(hWnd);
		if (!hdcScreen) {

			return;
		}
	}
	catch (const std::exception&)
	{

		return;
	}

	HDC hdcMem;
	try
	{
		hdcMem = CreateCompatibleDC(hdcScreen);
		if (!hdcMem) {
			ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
			return;
		}
	}
	catch (const std::exception&)
	{
		ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
		return;
	}

	HBITMAP hBitmap;
	try
	{
		hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
		if (!hBitmap) {
			ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
			DeleteDC(hdcMem);
			return;
		}
	}
	catch (const std::exception&)
	{
		ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
		DeleteDC(hdcMem);
		return;
	}

	SelectObject(hdcMem, hBitmap);

	if (!BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY)) {
		ReleaseDC(hWnd, hdcScreen);
		DeleteDC(hdcMem);
		DeleteObject(hBitmap);
		return;
	}

	BITMAPINFOHEADER bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height;
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	try
	{
		player = cv::Mat(height, width, CV_8UC3);
	}
	catch (const cv::Exception& e)
	{
		std::cout << e.what() << std::endl;
		DeleteDC(hdcMem);
		DeleteObject(hBitmap);
		ReleaseDC(hWnd, hdcScreen);
		return;
	}
	
	if (!GetDIBits(hdcScreen, hBitmap, 0, height, player.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS)) {
		ReleaseDC(hWnd, hdcScreen);
		DeleteDC(hdcMem);
		DeleteObject(hBitmap);
		return;
	}

	DeleteDC(hdcMem);
	DeleteObject(hBitmap);
	ReleaseDC(hWnd, hdcScreen);
}

bool Recognition::ComparOldPos(HWND& hWnd, cv::Mat& player) {
	POINT pos{};
	RECT rect;
	GetClientRect(hWnd, &rect);
	int width = rect.right;
	int height = rect.bottom;

	HDC hdcScreen;
	try
	{
		hdcScreen = GetDC(hWnd);
		if (!hdcScreen) {

			return false;
		}
	}
	catch (const std::exception&)
	{

		return false;
	}

	HDC hdcMem;
	try
	{
		hdcMem = CreateCompatibleDC(hdcScreen);
		if (!hdcMem) {
			ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
			return false;
		}
	}
	catch (const std::exception&)
	{
		ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
		return false;
	}

	HBITMAP hBitmap;
	try
	{
		hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
		if (!hBitmap) {
			ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
			DeleteDC(hdcMem);
			return false;
		}
	}
	catch (const std::exception&)
	{
		ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
		DeleteDC(hdcMem);
		return false;
	}


	SelectObject(hdcMem, hBitmap);
	
	if (!BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY)) {
		ReleaseDC(hWnd, hdcScreen);
		DeleteDC(hdcMem);
		DeleteObject(hBitmap);
		return false;
	}
	
	BITMAPINFOHEADER bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height;
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	cv::Mat screenShot;
	try
	{
		screenShot.create(height, width, CV_8UC3);
		if (screenShot.empty()) {
			ReleaseDC(hWnd, hdcScreen);
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			return false;
		}
	}
	catch (const cv::Exception& e)
	{
		std::cout << e.what() << std::endl;
		DeleteDC(hdcMem);
		DeleteObject(hBitmap);
		ReleaseDC(hWnd, hdcScreen);
		return false;
	}

	if (!GetDIBits(hdcScreen, hBitmap, 0, height, screenShot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS)) {
		ReleaseDC(hWnd, hdcScreen);
		DeleteDC(hdcMem);
		DeleteObject(hBitmap);
		return false;
	}

	// Liberar recursos
	ReleaseDC(hWnd, hdcScreen);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);

	if (player.empty()) {
		screenShot.release();
		return false;
	}

	int resultCols{};
	int resultRows{};
	cv::Mat resultImage;
	try
	{
		resultCols = screenShot.cols - player.cols + 1;
		resultRows = screenShot.rows - player.rows + 1;
		resultImage = cv::Mat(resultRows, resultCols, CV_32FC1);
	}
	catch (const cv::Exception& e)
	{
		std::cout << e.what() << std::endl;
		screenShot.release();
		resultImage.release();
		return false;
	}
	catch (const std::exception& e)
	{
		screenShot.release();
		resultImage.release();
		std::cout << e.what() << std::endl;
		return false;
	}

	cv::Mat thresholdedImage;
	std::vector<cv::Point> locations;
	try
	{
		cv::matchTemplate(screenShot, player, resultImage, cv::TM_CCOEFF_NORMED);
		cv::threshold(resultImage, thresholdedImage, 1, 1.0, cv::THRESH_TOZERO);
		cv::findNonZero(thresholdedImage, locations);
		if (locations.size() > 0) {
			screenShot.release();
			resultImage.release();
			thresholdedImage.release();
			return true;
		}
	}
	catch (const cv::Exception& e)
	{
		std::cout << e.what() << std::endl;
		screenShot.release();
		resultImage.release();
		thresholdedImage.release();
		return false;
	}

	screenShot.release();
	resultImage.release();
	thresholdedImage.release();

	DeleteDC(hdcMem);
	DeleteObject(hBitmap);
	ReleaseDC(hWnd, hdcScreen);
	return false;
}


//cv::Point bWood{ NULL };

bool comparar_pontos_por_y(const cv::Point& ponto1, const cv::Point& ponto2) {
	return ponto1.y < ponto2.y;
}

bool Recognition::WoodFarm(HWND& hWnd, FindPos FindWoodPos, bool debug) {
	RECT rect;
	GetClientRect(hWnd, &rect);
	int width = rect.right;
	int height = rect.bottom;

	HDC hdcScreen;
	try
	{
		hdcScreen = GetDC(hWnd);
		if (!hdcScreen) {

			return false;
		}
	}
	catch (const std::exception&)
	{

		return false;
	}


	HDC hdcMem;
	try
	{
		hdcMem = CreateCompatibleDC(hdcScreen);
		if (!hdcMem) {
			ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
			return false;
		}
	}
	catch (const std::exception&)
	{
		ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen antes de lançar a exceção
		return false;
	}

	HBITMAP hBitmap;
	try
	{
		hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
		if (!hBitmap) {
			ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
			DeleteDC(hdcMem);
			return false;
		}
	}
	catch (const std::exception&)
	{
		ReleaseDC(hWnd, hdcScreen); // Liberar hdcScreen e hdcMem antes de lançar a exceção
		DeleteDC(hdcMem);
		return false;
	}

	SelectObject(hdcMem, hBitmap);

	if (!BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY)) {
		ReleaseDC(hWnd, hdcScreen);
		DeleteDC(hdcMem);
		DeleteObject(hBitmap);
		return false;
	}

	BITMAPINFOHEADER bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height;
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	cv::Mat screenShot;
	try
	{
		screenShot.create(height, width, CV_8UC3);
		if (screenShot.empty()) {
			ReleaseDC(hWnd, hdcScreen);
			DeleteDC(hdcMem);
			DeleteObject(hBitmap);
			return false;
		}
	}
	catch (const cv::Exception& e)
	{
		std::cout << e.what() << std::endl;
		DeleteDC(hdcMem);
		DeleteObject(hBitmap);
		ReleaseDC(hWnd, hdcScreen);
		return false;
	}

	if (!GetDIBits(hdcScreen, hBitmap, 0, height, screenShot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS)) {
		ReleaseDC(hWnd, hdcScreen);
		DeleteDC(hdcMem);
		DeleteObject(hBitmap);
		return false;
	}

	// Liberar recursos
	ReleaseDC(hWnd, hdcScreen);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);

	for (auto& image : Images) {
		if (image.second != ImageType::street) {
			continue;
		}

		cv::Mat subImage = cv::imread(image.first);

		if (subImage.empty()) {
			continue;
		}

		int resultCols{};
		int resultRows{};
		cv::Mat resultImage;
		try
		{
			resultCols = screenShot.cols - subImage.cols + 1;
			resultRows = screenShot.rows - subImage.rows + 1;
			resultImage = cv::Mat(resultRows, resultCols, CV_32FC1);
		}
		catch (const cv::Exception& e)
		{
			std::cout << e.what() << std::endl;
			continue;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			continue;
		}

		cv::Mat thresholdedImage;
		std::vector<cv::Point> locations;
		try
		{
			cv::matchTemplate(screenShot, subImage, resultImage, cv::TM_CCOEFF_NORMED);
			cv::threshold(resultImage, thresholdedImage, 0.53, 1.0, cv::THRESH_TOZERO);
			cv::findNonZero(thresholdedImage, locations);
		}
		catch (const cv::Exception& e)
		{
			std::cout << e.what() << std::endl;
			continue;
		}

		try
		{
			if (locations.size() > 0) {
				if (debug) {
					for (const auto& loc : locations) {
						cv::rectangle(screenShot, loc, cv::Point(loc.x + subImage.cols, loc.y + subImage.rows), cv::Scalar(0, 255, 0), 2);
						int centerX = loc.x + subImage.cols / 2;
						int centerY = loc.y + subImage.rows / 2;
						cv::Point center(centerX, centerY);
						cv::circle(screenShot, center, 5, cv::Scalar(0, 0, 255), -1);
						//cv::line(screenShot, center, cv::Point(center.x + pos.x, center.y + pos.y), cv::Scalar(255, 0, 0), 2);
					}
				}

				switch (FindWoodPos)
				{
				case 0: {
					//652 378
					auto min = std::min_element(locations.begin(), locations.end(), comparar_pontos_por_y);
					Recognition::bestWoodVec.push_back(*min);
					break;
				}
				case 1: {
					auto max = std::max_element(locations.begin(), locations.end(), comparar_pontos_por_y);
					Recognition::bestWoodVec.push_back(*max);
					break;
				}
				default:
					break;
				}
			}
		}
		catch (const cv::Exception& e)
		{
			std::cout << e.what() << std::endl;
			continue;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			continue;
		}
	}


	if (debug) {
		try
		{
			cv::namedWindow("Debug", cv::WINDOW_NORMAL);
			cv::imshow("Debug", screenShot);

			// Pressione 'q' para sair do loop
			if (cv::waitKey(1) == 'q') {

			}

		}
		catch (const cv::Exception& e)
		{
			std::cerr << "Exceção capturada: " << e.what() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	switch (FindWoodPos)
	{
	case 0: {
		//652 378
		if (bestWoodVec.size() > 0) {
			auto min = std::min_element(bestWoodVec.begin(), bestWoodVec.end(), comparar_pontos_por_y);
			Recognition::bestWood = *min;
			Recognition::bestWoodVec.clear();
			return true;
		}

		return false;
		break;
	}
	case 1: {
		if (bestWoodVec.size() > 0) {
			auto max = std::max_element(bestWoodVec.begin(), bestWoodVec.end(), comparar_pontos_por_y);
			Recognition::bestWood = *max;
			Recognition::bestWoodVec.clear();
			return true;
		}
		return false;
		break;
	}
	default:
		break;
	}
	return false;
}

