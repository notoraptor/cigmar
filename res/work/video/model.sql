CREATE TABLE IF NOT EXISTS library (
    library_id INTEGER PRIMARY KEY AUTOINCREMENT,
    library_name VARCHAR(512) NOT NULL UNIQUE,
    thumbnail_extension VARCHAR(32) NOT NULL
);

CREATE TABLE IF NOT EXISTS video_folder (
    video_folder_id INTEGER PRIMARY KEY AUTOINCREMENT,
    windows_absolute_path TEXT,
    unix_absolute_path TEXT,
    library_id INTEGER REFERENCES library(library_id) ON DELETE CASCADE,
    UNIQUE (library_id, windows_absolute_path),
    UNIQUE (library_id, unix_absolute_path),
    CHECK (windows_absolute_path IS NOT NULL OR unix_absolute_path IS NOT NULL)
);

CREATE TABLE IF NOT EXISTS video (
    video_id INTEGER PRIMARY KEY AUTOINCREMENT,
    date_added_microseconds BIGINT NOT NULL,
    video_size UNSIGNED BIG INT NOT NULL,
    format VARCHAR(256),
    audiocodec VARCHAR(256),
    videocodec VARCHAR(256),
    samplerate DOUBLE,
    framerate DOUBLE,
    duration DOUBLE NOT NULL,
    width UNSIGNED INTEGER NOT NULL,
    height UNSIGNED INTEGER NOT NULL,
    relative_path TEXT NOT NULL,
    video_hash VARCHAR(1024) NOT NULL,
    video_folder_id INTEGER REFERENCES video_folder(video_folder_id) ON DELETE CASCADE,
    UNIQUE (video_folder_id, relative_path)
);

CREATE TABLE IF NOT EXISTS property_type (
    property_type_id INTEGER PRIMARY KEY AUTOINCREMENT,
    property_type_name VARCHAR(32) NOT NULL UNIQUE
);


CREATE TABLE IF NOT EXISTS unique_property (
    unique_property_id INTEGER PRIMARY KEY AUTOINCREMENT,
    unique_property_name TEXT NOT NULL UNIQUE,
    default_value TEXT,
    property_type_id INTEGER REFERENCES property_type(property_type_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS multiple_property (
    multiple_property_id INTEGER PRIMARY KEY AUTOINCREMENT,
    multiple_property_name TEXT NOT NULL UNIQUE,
    property_type_id INTEGER REFERENCES property_type(property_type_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS unique_property_to_video (
    video_id INTEGER REFERENCES video(video_id) ON DELETE CASCADE,
    unique_property_id INTEGER REFERENCES unique_property(unique_property_id) ON DELETE CASCADE,
    value TEXT,
    PRIMARY KEY (video_id, unique_property_id)
);

CREATE TABLE IF NOT EXISTS multiple_property_to_video (
    video_id INTEGER REFERENCES video(video_id) ON DELETE CASCADE,
    multiple_property_id INTEGER REFERENCES multiple_property(multiple_property_id) ON DELETE CASCADE,
    value TEXT,
    PRIMARY KEY (video_id, multiple_property_id, value)
);

INSERT OR IGNORE INTO property_type (property_type_name) VALUES ('bool'), ('int'), ('uint'), ('double'), ('string')
