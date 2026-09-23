SELECT 
    ss.student_id,
    ss.student_name,
    ss.subject_name,
    COUNT(e.student_id) AS attended_exams
FROM 
    (
        SELECT *
        FROM Students 
        CROSS JOIN Subjects
    ) AS ss
LEFT JOIN Examinations AS e
    ON ss.student_id = e.student_id
    AND ss.subject_name = e.subject_name
GROUP BY
    ss.student_id,
    ss.student_name,
    ss.subject_name
ORDER BY
    ss.student_id,
    ss.subject_name;