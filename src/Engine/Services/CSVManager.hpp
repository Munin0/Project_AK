// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Extern/rapidcsv.h"
// | -------------------------------
#include <map>
#include <memory>
#include <string>
#include <vector>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Loads CSV files and reads text from them.
   *
   * CSVManager keeps every loaded CSV document under a string key and
   * provides helpers to read texts from them by language, so it can be used
   * for dialogs and localization.
   *
   * The CSV files are parsed with rapidcsv.
   */
  class CSVManager
  {
    public:
      /**
       * @brief Loads a CSV file and stores it under a key.
       *
       * @param[in] path  Path of the CSV file to load.
       * @param[in] key   Key used to refer to this document in the other
       *                  functions of the manager.
       * @param[in] colID Index of the row that contains the column labels
       *                  (rapidcsv's column name index). Defaults to 0.
       * @param[in] rowID Index of the column that contains the row labels
       *                  (rapidcsv's row name index). Defaults to 0.
       */
      void Load(const std::string& path, const std::string& key, int colID = 0, int rowID = 0);
      /**
       * @brief Returns a text from a loaded CSV.
       *
       * @param[in] language Language to read, matching a column label of the CSV.
       * @param[in] dialog   Identifier of the dialog to read, matching a row
       *                     label of the CSV.
       * @param[in] key      Key the CSV document was loaded with.
       * @return The text in the given language for the given dialog.
       */
      std::string GetText(const std::string& language, const std::string& dialog, const std::string& key);
      /**
       * @brief Returns a range of texts from a loaded CSV.
       *
       * Reads the texts of a single language for a range of dialogs.
       *
       * @param[in] language Language to read, matching a column label of the CSV.
       * @param[in] from     Row label where the range starts.
       * @param[in] where    Row label where the range ends.
       * @param[in] key      Key the CSV document was loaded with.
       * @return The texts of the range, in the order they appear in the CSV.
       */
      std::vector<std::string> GetRangeText(const std::string& language, const std::string& from, const std::string& where, const std::string& key);
      /**
       * @brief Releases every loaded CSV document.
       *
       * Any document loaded before this call must be loaded again with
       * Load() before reading from it.
       */
      void Clear();
    private:
      /**
       * @brief Returns a loaded CSV document.
       *
       * @param[in] key Key the document was loaded with.
       * @return Pointer to the document. It is owned by the manager.
       */
      rapidcsv::Document* GetDoc(const std::string& key);
      /// @brief Loaded CSV documents, indexed by key.
      std::map<std::string, std::unique_ptr<rapidcsv::Document>> m_csv;
  };
}
